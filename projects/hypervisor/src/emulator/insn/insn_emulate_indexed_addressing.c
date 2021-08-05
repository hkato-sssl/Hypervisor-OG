/*
 * emulator/insn/insn_emulate_indexed_addressing.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/emulator/insn.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t emulate_indexed_addressing_ldr(const struct insn *insn)
{
    if (insn->op.ldr.flag.post != 0) {
        insn->vpc->regs[VPC_X0 + insn->op.ldr.gpr.src] += insn->op.ldr.offset;
    } else if (insn->op.ldr.flag.pre != 0) {
        insn->vpc->regs[VPC_X0 + insn->op.ldr.gpr.src] = insn->op.ldr.va;
    }

    return SUCCESS;
}

static errno_t emulate_indexed_addressing_str(const struct insn *insn)
{
    if (insn->op.str.flag.post != 0) {
        insn->vpc->regs[VPC_X0 + insn->op.str.gpr.dst] += insn->op.str.offset;
    } else if (insn->op.str.flag.pre != 0) {
        insn->vpc->regs[VPC_X0 + insn->op.str.gpr.dst] = insn->op.str.va;
    }

    return SUCCESS;
}

errno_t insn_emulate_indexed_addressing(const struct insn *insn)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        ret = emulate_indexed_addressing_ldr(insn);
    } else if (insn->type == INSN_TYPE_STR) {
        ret = emulate_indexed_addressing_str(insn);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}
