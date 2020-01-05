/*
 * vpc/vpc_emulate_ldr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void vpc_emulate_ldrb(const struct insn *insn, uint64_t d)
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

void vpc_emulate_ldrh(const struct insn *insn, uint64_t d)
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

void vpc_emulate_ldrw(const struct insn *insn, uint64_t d)
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

