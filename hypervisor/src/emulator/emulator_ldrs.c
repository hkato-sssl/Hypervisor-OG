/*
 * emulator/emulator_ldrs.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t emulator_ldrsb(uint64_t d, const struct insn *insn)
{
    if ((d & BIT(7)) != 0) {
        if (insn->op.ldr.flag.a32 != 0) {
            d = (d | 0xffffff00ULL) & 0xffffffffULL;
        } else {
            d |= 0xffffffffffffff00ULL;
        }
    }

    return d; 
}

uint64_t emulator_ldrsw(uint64_t d, const struct insn *insn)
{
    if ((d & BIT(31)) != 0) {
        d |= 0xffffffff00000000ULL;
    }

    return d; 
}

