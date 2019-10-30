/*
 * vpc/vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

volatile void vpc_switch_to_el1(uint64_t *regs);

/* variables */

/* functions */

volatile void vpc_launch(struct vpc *vpc)
{
    vpc->flag.launched = true;
    vpc_load_ctx_system_register(vpc->regs);
    vpc_load_ctx_fpu(vpc->regs);
    vpc_switch_to_el1(vpc->regs);
}

volatile void vpc_resume(struct vpc *vpc)
{
    vpc_switch_to_el1(vpc->regs);
}

