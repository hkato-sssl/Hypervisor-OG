/*
 * insn/insn_generate_ldr_str.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/system_register/esr_el2_iss.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t parse_instruction(struct insn *insn, struct vpc *vpc, uint64_t va, uint64_t ipa)
{
    return -ENOSYS;
}

static errno_t parse_system_register(struct insn *insn, struct vpc *vpc, uint64_t va, uint64_t ipa)
{
    uint64_t esr;
    uint64_t sas;

    insn->vpc = vpc;
    esr = vpc->regs[VPC_ESR_EL2];
    insn->type = ((esr & ISS_DATA_ABORT_WnR) != 0) ? INSN_TYPE_STR : INSN_TYPE_LDR;
    insn->op.ldr.va = va;
    insn->op.ldr.ipa = ipa;

    sas = EXTRACT_ISS_DATA_ABORT_SAS(esr);
    insn->op.ldr.size = 1 << sas;

    if (insn->type == INSN_TYPE_LDR) {
        insn->op.ldr.gpr.dst = (uint8_t)EXTRACT_ISS_DATA_ABORT_SRT(esr);
    } else {
        insn->op.ldr.gpr.src = (uint8_t)EXTRACT_ISS_DATA_ABORT_SRT(esr);
    }

    insn->op.ldr.flag.sign = ((esr & ISS_DATA_ABORT_SSE) == 0) ? 0 : 1;
    insn->op.ldr.flag.a32 = ((esr & ISS_DATA_ABORT_SF) == 0) ? 0 : 1;

    return SUCCESS;
}

static errno_t parse(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;
    uint64_t va;
    uint64_t ipa;
    uint64_t mask;

    memset(insn, 0, sizeof(*insn));

    mask = UNIT_VM_TRAP_REGION - 1;
    va = vpc->regs[VPC_FAR_EL2];
    ipa = (vpc->regs[VPC_HPFAR_EL2] << 8) | (va & mask);

    if ((vpc->regs[VPC_ESR_EL2] & ISS_DATA_ABORT_ISV) == 0) {
        ret = parse_instruction(insn, vpc, va, ipa);
    } else {
        ret = parse_system_register(insn, vpc, va, ipa);
    }

    return ret;
}

static bool is_emulatable(const struct vpc *vpc)
{
    bool ret;
    uint64_t esr;
    uint64_t dfsc;

    esr = vpc->regs[VPC_ESR_EL2];
    if ((esr & (ISS_DATA_ABORT_EA | ISS_DATA_ABORT_CM | ISS_DATA_ABORT_S1PTW)) == 0) {
        dfsc = EXTRACT_ISS_DATA_ABORT_DFSC(esr);
        if ((dfsc >= 9) && (dfsc <= 11)) {
            /* Access flag fault */
            ret = true;
        } else {
            ret = false;
        }
    } else {
        ret = false;
    }

    return ret;
}

errno_t insn_generate_ldr_str(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;

    if (is_emulatable(vpc)) {
        ret = parse(insn, vpc);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}

