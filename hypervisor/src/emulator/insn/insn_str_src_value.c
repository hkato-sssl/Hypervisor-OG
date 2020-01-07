/*
 * emulate/insn/insn_str_src_value.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/assert.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t insn_str_src_value(const struct insn *insn)
{
    uint64_t val;
    uint64_t mask;
    uint8_t gpr;

    SYSTEM_ASSERT(insn->type == INSN_TYPE_STR);
    
    gpr = insn->op.str.gpr.src;
    if (gpr < 31) {
        val = insn->vpc->regs[gpr];
    } else {
        val = 0;
    }
    if (insn->op.str.size < 8) {
        mask = (1ULL << (insn->op.str.size * 8)) - 1;
        val &= mask;
    }

    return val;
}

