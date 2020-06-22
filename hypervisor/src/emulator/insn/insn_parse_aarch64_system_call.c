/*
 * emulate/insn/insn_parse_aarch64_system_call.c
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

static errno_t parse(struct insn *insn, struct vpc *vpc, enum insn_operand_type type)
{
    memset(insn, 0, sizeof(*insn));
    insn->vpc = vpc;
    insn->type = type;
    insn->op.system_call.imm = (uint16_t)(vpc->regs[VPC_ESR_EL2]);

    return SUCCESS;
}

errno_t insn_parse_aarch64_svc(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;

    ret = parse(insn, vpc, INSN_TYPE_SVC);

    return ret;
}

errno_t insn_parse_aarch64_hvc(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;

    ret = parse(insn, vpc, INSN_TYPE_HVC);

    return ret;
}

errno_t insn_parse_aarch64_smc(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;

    ret = parse(insn, vpc, INSN_TYPE_SMC);

    return ret;
}

