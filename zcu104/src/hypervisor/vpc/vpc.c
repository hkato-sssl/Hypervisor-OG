/*
 * vpc/vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "hypervisor/tls.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

volatile void vpc_switch_to_el1(uint64_t *regs);

/* variables */

/* functions */

errno_t vpc_launch(struct vpc *vpc)
{
    tls_write(TLS_CURRENT_VPC_REGS, (uint64_t)vpc->regs);
    tls_write(TLS_CURRENT_VPC, (uint64_t)vpc);

    vpc->boolean.launched = true;
    vpc_load_ctx_system_register(vpc->regs);
    vpc_load_ctx_fpu(vpc->regs);
    vpc_switch_to_el1(vpc->regs);

    return SUCCESS;
}

errno_t vpc_resume(struct vpc *vpc)
{
    errno_t ret;

    if (vpc->boolean.launched) {
    	vpc_switch_to_el1(vpc->regs);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

