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

static uint64_t str_src_value(const struct insn *insn)
{
    uint64_t d;
    uint64_t mask;

    d = insn->vpc->regs[insn->op.str.gpr.src];
    if (insn->op.str.size < 8) {
        mask = (1ULL << (insn->op.str.size * 8)) - 1;
        d &= mask;
    }

    return d;
}

uint64_t insn_str_src_value(const struct insn *insn)
{
    uint64_t val;

    if (insn->type == INSN_TYPE_STR) {
        val = str_src_value(insn);
    } else {
        SYSTEM_ASSERT(0);
        val = 0;
    }

    return val;
}

