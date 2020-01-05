/*
 * vpc/vpc_emulate_ldr.c
 *
 * (C) 2019 Hidekazu Kato
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

static void emulate_ldrb(const struct insn *insn, uint64_t d)
{
    if (insn->op.ldr.flag.a32 == 0) {
        if ((insn->op.ldr.flag.sign != 0) && ((d & BIT(7)) != 0)) {
            d |= 0xffffffffffffff00ULL;
        } else {
            d &= BITS(7, 0);
        }
    } else {
        d &= BITS(7, 0);
        if ((insn->op.ldr.flag.sign != 0) && ((d & BIT(7)) != 0)) {
            d |= 0x00000000ffffff00UL;
        }
    }

    insn->vpc->regs[insn->op.ldr.gpr.dst] = d;
}

static void emulate_ldrh(const struct insn *insn, uint64_t d)
{
    if (insn->op.ldr.flag.a32 == 0) {
        if ((insn->op.ldr.flag.sign != 0) && ((d & BIT(15)) != 0)) {
            d |= 0xffffffffffff0000ULL;
        } else {
            d &= BITS(15, 0);
        }
    } else {
        d &= BITS(15, 0);
        if ((insn->op.ldr.flag.sign != 0) && ((d & BIT(15)) != 0)) {
            d |= 0x00000000ffff0000ULL;
        }
    }

    insn->vpc->regs[insn->op.ldr.gpr.dst] = d;
}

static void emulate_ldrw(const struct insn *insn, uint64_t d)
{
    if (insn->op.ldr.flag.a32 == 0) {
        if ((insn->op.ldr.flag.sign != 0) && ((d & BIT(31)) != 0)) {
            d |= 0xffffffff00000000ULL;
        } else {
            d &= BITS(31, 0);
        }
    } else {
        d &= BITS(31, 0);
    }

    insn->vpc->regs[insn->op.ldr.gpr.dst] = d;
}

static void emulate_ldr(const struct insn *insn, uint64_t d)
{
    insn->vpc->regs[insn->op.ldr.gpr.dst] = d;
}

void vpc_emulate_ldr(const struct insn *insn, uint64_t d)
{
    SYSTEM_ASSERT(insn->type == INSN_TYPE_LDR);

    switch (insn->op.ldr.size) {
    case 1:
        emulate_ldrb(insn, d);
        break;
    case 2:
        emulate_ldrh(insn, d);
        break;
    case 4:
        emulate_ldrw(insn, d);
        break;
    case 8:
        emulate_ldr(insn, d);
        break;
    default:
        SYSTEM_ASSERT(0);
        break;
    }

    vpc_emulate_post_operation(insn);
}

