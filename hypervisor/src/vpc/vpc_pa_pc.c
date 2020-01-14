/*
 * vpc/vpc_pc_pa.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/vpc.h"
#include "vpc_local.h"
#include "driver/aarch64/at.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vpc_pa_pc(const struct vpc *vpc, uint64_t *pc)
{
    errno_t ret;
    uint64_t par;
    uint64_t va;

    va = vpc->regs[VPC_PC];
    if (is_el0(vpc)) {
        par = aarch64_at_s12e0r(va);
    } else {
        par = aarch64_at_s12e1r(va);
    }

    if ((par & BIT(0)) == 0) {
        *pc = (par & BITS(47, 12)) | (va & BITS(11, 0));
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

