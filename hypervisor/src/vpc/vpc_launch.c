/*
 * vpc/vpc_launch.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
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

static errno_t call_post_hook(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->post.launch != NULL)) {
        ret = (*(vpc->hook->post.launch))(vpc);
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t init_system_register(struct vpc *vpc)
{
    uint64_t d;

    vpc->regs[VPC_SCTLR_EL1] = SCTLR_EL1_RES1 | DEFAULT_SCTLR_EL1;

    d = aarch64_read_midr_el1();
    vpc->regs[VPC_VPIDR_EL2] = d;
    vpc->regs[VPC_VMPIDR_EL2] = BIT(31) | vpc->proc_no;

    return SUCCESS;
}

static errno_t setup_aarch64(struct vpc *vpc)
{
    errno_t ret;

    /* SPSR:
     *    D, A, I, F = 1
     *    M[4:0] = 0x05 - AArch64 EL1h
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_D | PSTATE_A | PSTATE_I | PSTATE_F | 0x05;
    vpc->regs[VPC_HCR_EL2] = HCR_RW | HCR_IMO | HCR_VM;

    ret = init_system_register(vpc);

    return ret;
}

static errno_t setup_aarch32(struct vpc *vpc)
{
    errno_t ret;

    /* SPSR:
     *    I, F = 1 - IRQ and FIQ are masked
     *    M[4:0] = 0x13 - AArch32 Supervisor mode
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_I | PSTATE_F | 0x13;
    vpc->regs[VPC_HCR_EL2] = HCR_IMO | HCR_VM;

    ret = init_system_register(vpc);

    return ret;
}

static errno_t launch(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vm *vm;

    vpc->regs[VPC_PC] = boot->pc;
    vpc->regs[VPC_SP_EL1] = boot->sp;

    vm = vpc->vm;
    vpc->regs[VPC_VTTBR_EL2] = aarch64_stage2_vttbr_el2(vm->stage2);
    vpc->regs[VPC_VTCR_EL2] = aarch64_stage2_vtcr_el2(vm->stage2);

    if (boot->arch == VPC_ARCH_AARCH64) {
	    setup_aarch64(vpc);
    } else {
	    setup_aarch32(vpc);
    }

    thread_write_tls(TLS_CURRENT_VPC_REGS, (uint64_t)vpc->regs);
    thread_write_tls(TLS_CURRENT_VPC, (uint64_t)vpc);
    thread_write_tls(TLS_CURRENT_VM, (uint64_t)(vpc->vm));

    vpc->boolean.launched = true;
    vpc_load_ctx_system_register(vpc->regs);
    vpc_load_ctx_fpu(vpc->regs);

    ret = vpc_switch_to_el1(vpc->regs);
    if (ret == SUCCESS) {
        ret = call_post_hook(vpc);
    } else {
        /* In this case, ignore the return value. */
        call_post_hook(vpc);
    }

    return ret;
}

static errno_t call_previous_hook(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->previous.launch != NULL)) {
        ret = (*(vpc->hook->previous.launch))(vpc);
        if (ret == SUCCESS) {
            ret = launch(vpc, boot);
        }
    } else {
        ret = launch(vpc, boot);
    }

    return ret;
}

static bool is_valid_arch(enum vpc_arch arch)
{
    bool valid;

    if ((arch == VPC_ARCH_AARCH64) || (arch == VPC_ARCH_AARCH32)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t vpc_launch(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    assert((vpc != NULL) && (boot != NULL));

    if (vpc->boolean.launched) {
        ret = -EBUSY;
    } else if (! is_valid_arch(boot->arch)) {
        ret = -EINVAL;
    } else {
        ret = vm_map_proc_no(vpc->vm, vpc);
        if (ret == SUCCESS) {
            ret = call_previous_hook(vpc, boot);
        }
    }

    return ret;
}

