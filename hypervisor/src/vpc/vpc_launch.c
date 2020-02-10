/*
 * vpc/vpc_launch.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "driver/arm.h"
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/hcr_el2.h"
#include "hypervisor/thread.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "vpc_local.h"

/* defines */

#define DEFAULT_SCTLR_EL1   (SCTLR_nTWE | SCTLR_nTWI | SCTLR_CP15BEN | SCTLR_SA0 | SCTLR_SA) 

/* types */

/* prototypes */

/* variables */

/* functions */

static void initialize_el1(struct vpc *vpc)
{
    uint64_t d;

    vpc->regs[VPC_SCTLR_EL1] = SCTLR_EL1_RES1 | DEFAULT_SCTLR_EL1;

    d = aarch64_read_midr_el1();
    vpc->regs[VPC_VPIDR_EL2] = d;
    vpc->regs[VPC_VMPIDR_EL2] = BIT(31) | vpc->proc_no;
}

static errno_t switch_to_guest_context(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vm *vm;

    if (boot != NULL) {
        vpc_set_boot_parameters(vpc, boot);
    }
    initialize_el1(vpc);

    vm = vpc->vm;
    vpc->regs[VPC_VTTBR_EL2] = aarch64_stage2_vttbr_el2(vm->stage2);
    vpc->regs[VPC_VTCR_EL2] = aarch64_stage2_vtcr_el2(vm->stage2);
    vpc->regs[VPC_HCR_EL2] |= HCR_EL2_TSC;

    thread_write_tls(TLS_CURRENT_VPC_REGS, (uint64_t)vpc->regs);
    thread_write_tls(TLS_CURRENT_VPC, (uint64_t)vpc);
    thread_write_tls(TLS_CURRENT_VM, (uint64_t)(vpc->vm));

    vpc_load_ctx_system_register(vpc->regs);
    vpc_load_ctx_fpu(vpc->regs);
    vpc_set_status(vpc, VPC_STATUS_RUN);

    ret = vpc_switch_to_el1(vpc->regs);

    return ret;
}

static errno_t call_previous_hook(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->launch.previous != NULL)) {
        ret = (*(vpc->hook->launch.previous))(vpc);
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t call_post_hook(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->launch.post != NULL)) {
        ret = (*(vpc->hook->launch.post))(vpc);
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t launch(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    errno_t post;

    ret = call_previous_hook(vpc);
    if (ret == SUCCESS) {
        ret = switch_to_guest_context(vpc, boot);
        post = call_post_hook(vpc);
        if ((ret == SUCCESS) && (post != SUCCESS)) {
            ret = post;
        }
    }

    return ret;
}

static errno_t validate_parameters(const struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    enum vpc_status status;

    ret = validate_vpc(vpc);
    if (ret == SUCCESS) {
        status = vpc_watch_status(vpc);
        switch (status) {
        case VPC_STATUS_DOWN:
            if (boot == NULL) {
                ret = -EINVAL;
            } else if (boot->arch != VPC_ARCH_AARCH64) {
                ret = -EINVAL;
            } else {
                ret = SUCCESS;
            }
            break;
        case VPC_STATUS_WAKEUP:
            if (boot == NULL) {
                ret = SUCCESS;
            } else {
                ret = -EINVAL;
            }
            break;
        case VPC_STATUS_RUN:
            ret = -EBUSY;
            break;
        default:
            ret = -EPERM;
        }
    }

    return ret;
}

errno_t vpc_launch(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    assert(vpc != NULL);

    ret = validate_parameters(vpc, boot);
    if (ret == SUCCESS) {
        ret = launch(vpc, boot);
    }

    return ret;
}

