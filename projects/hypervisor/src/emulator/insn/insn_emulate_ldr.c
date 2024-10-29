// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/insn/insn_emulate_ldr.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/emulator/insn.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/assert.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint64_t unsigned_value(const struct insn *insn, uint64_t d)
{
    switch (insn->op.ldr.size) {
    case 1:
        d &= BITS(7, 0);
        break;
    case 2:
        d &= BITS(15, 0);
        break;
    case 4:
        d &= BITS(31, 0);
        break;
    case 8:
        break;
    default:
        system_error(__FILE__, __LINE__, "size is set to an illegal value.");
    }

    return d;
}

static inline uint64_t signed_extension(uint64_t d, unsigned int msb)
{
    if ((d & BIT(msb)) == 0) {
        d &= BITS(msb, 0);
    } else {
        d |= BITS(63, (msb + 1));
    }

    return d;
}

static uint64_t signed_value(const struct insn *insn, uint64_t d)
{
    switch (insn->op.ldr.size) {
    case 1:
        d = signed_extension(d, 7);
        break;
    case 2:
        d = signed_extension(d, 15);
        break;
    case 4:
        d = signed_extension(d, 31);
        break;
    case 8:
    default:
        system_error(__FILE__, __LINE__, "size is set to an illegal value.");
    }

    if (insn->op.ldr.flag.wreg != 0) {
        d &= BITS(31, 0);
    }

    return d;
}

static errno_t emulate_post_operation(const struct insn *insn)
{
    errno_t ret;

    ret = insn_emulate_indexed_addressing(insn);
    if (ret == SUCCESS) {
        ret = vpc_update_pc(insn->vpc);
    }

    return ret;
}

static errno_t emulate_ldr(const struct insn *insn, uint64_t d)
{
    errno_t ret;

    if (insn->op.ldr.flag.sign == 0) {
        d = unsigned_value(insn, d);
    } else {
        d = signed_value(insn, d);
    }
    insn->vpc->regs[VPC_X0 + insn->op.ldr.gpr.dst] = d;

    ret = emulate_post_operation(insn);

    return ret;
}

errno_t insn_emulate_ldr(const struct insn *insn, uint64_t d)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (insn->op.ldr.gpr.dst < 31) {
            ret = emulate_ldr(insn, d);
        } else {
            ret = emulate_post_operation(insn);
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}
