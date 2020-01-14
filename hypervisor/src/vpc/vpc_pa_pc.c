/*
 * vpc/vpc_pc_pa.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vpc_pa_pc(const struct vpc *vpc, uint64_t *pc)
{
    errno_t ret;
    uint64_t va;

    va = vpc->regs[VPC_PC];
    ret = vpc_va_to_pa(vpc, pc, va);

    return ret;
}

