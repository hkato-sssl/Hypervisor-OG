/*
 * vpc/vpc_load_to_gpr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void vpc_load_to_gpr_b(const struct vpc_memory_access *access, uint64_t d)
{
    if ((access->request.flag.sign != 0) && ((d & BIT(7)) != 0)) {
        d |= 0xffffffffffffff00ULL;
    } else {
        d &= BITS(7, 0);
    }

    access->vpc->regs[access->request.gpr] = d;
}

void vpc_load_to_gpr_h(const struct vpc_memory_access *access, uint64_t d)
{
    if ((access->request.flag.sign != 0) && ((d & BIT(15)) != 0)) {
        d |= 0xffffffffffff0000ULL;
    } else {
        d &= BITS(15, 0);
    }

    access->vpc->regs[access->request.gpr] = d;
}

void vpc_load_to_gpr_w(const struct vpc_memory_access *access, uint64_t d)
{
    if ((access->request.flag.sign != 0) && ((d & BIT(31)) != 0)) {
        d |= 0xffffffff00000000ULL;
    } else {
        d &= BITS(31, 0);
    }

    access->vpc->regs[access->request.gpr] = d;
}

