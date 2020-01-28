/*
 * emulate/insn/insn_parse_aarch64_hvc.c
 *
 * (C) 2020 Hidekazu Kato
 *
 */

#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t insn_parse_aarch64_hvc(struct insn *insn, struct vpc *vpc)
{
    memset(insn, 0, sizeof(*insn));
    insn->type = INSN_TYPE_HVC;
    insn->op.hvc.imm = (uint16_t)(vpc->regs[VPC_ESR_EL2] & BITS(15, 0));

    return SUCCESS;
}

