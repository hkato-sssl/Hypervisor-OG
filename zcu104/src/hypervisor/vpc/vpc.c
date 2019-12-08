/*
 * vpc/vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "driver/aarch64/system_register.h"
#include "hypervisor/tls.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

errno_t vpc_switch_to_el1(uint64_t *regs);

/* variables */

/* functions */

static errno_t launch(struct vpc *vpc)
{
    errno_t ret;

    tls_write(TLS_CURRENT_VPC_REGS, (uint64_t)vpc->regs);
    tls_write(TLS_CURRENT_VPC, (uint64_t)vpc);
    tls_write(TLS_CURRENT_VM, (uint64_t)(vpc->owner));

    vpc->boolean.launched = true;
    vpc_load_ctx_system_register(vpc->regs);
    vpc_load_ctx_fpu(vpc->regs);
    ret = vpc_switch_to_el1(vpc->regs);

    return ret;
}

errno_t vpc_launch(struct vpc *vpc)
{
    errno_t ret;
    struct vm *owner;

    if (! vpc->boolean.launched) {
        owner = vpc->owner;
        vpc->regs[VPC_VTTBR_EL2] = aarch64_stage2_vttbr_el2(owner->stage2);
        vpc->regs[VPC_VTCR_EL2] = aarch64_stage2_vtcr_el2(owner->stage2);
        ret = launch(vpc);
    } else {
        ret = -EBUSY;
    }

    return ret;
}

errno_t vpc_resume(struct vpc *vpc)
{
    errno_t ret;

    if (vpc->boolean.launched) {
        ret = vpc_switch_to_el1(vpc->regs);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

