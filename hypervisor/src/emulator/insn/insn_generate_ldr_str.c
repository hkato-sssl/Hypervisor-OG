/*
 * insn/insn_generate_ldr_str.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register/esr_el2_iss.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "driver/aarch64/system_register.h"


/* defines */

#define OP_LDR_POST         0x38400400
#define OP_LDRS_POST        0x38800400
#define OP_LDRS_POST_W      0x38c00400
#define OP_LDR_WB           0x38400c00
#define OP_LDRS_WB          0x38800000
#define OP_LDRS_WB_W        0x38c00c00
#define OP_STR_POST         0x38000400
#define OP_STR_WB           0x38000c00

/* types */

/* prototypes */

/* variables */

/* functions */

/* Instruction code
 *
 * post-index
 * 
 *      10987654321098765432109876543210
 * LDR  SS111000010IIIIIIIII01sssssddddd  0x38400400
 * STR  SS111000000IIIIIIIII01sssssddddd  0x38000400
 * 
 * SS: 00 - 8-bit
 *     01 - 16-bit
 *     10 - 32-bit
 *     11 - 64-bit
 * 
 *      10987654321098765432109876543210
 * LDRS SS1110001A0IIIIIIIII01sssssddddd  0x38c00400(W), 0x38800400(X)
 * 
 * A: 1 - 32-bit variant
 *    0 - 64-bit variant
 * 
 * SS: 00 - 8-bit
 *     01 - 16-bit
 *     10 - 32-bit(always A=0)
 * 
 * pre-index, write-back
 *  
 *      10987654321098765432109876543210
 * LDR  SS111000010IIIIIIIII11sssssddddd  0x38400c00
 * STR  SS111000000IIIIIIIII11sssssddddd  0x38000c00
 * 
 * SS: 00 - 8-bit
 *     01 - 16-bit
 *     10 - 32-bit
 *     11 - 64-bit
 * 
 *      10987654321098765432109876543210
 * LDRS SS1110001A0IIIIIIIII11sssssddddd  0x38c00c00(W), 0x38800c00(X)
 * 
 * A: 1 - 32-bit variant
 *    0 - 64-bit variant
 * 
 * SS: 00 - 8-bit
 *     01 - 16-bit
 *     10 - 32-bit(always A=0)
 */

static uint8_t aarch64_op_size(uint32_t code)
{
    uint8_t size;

    size = (uint8_t)BF_EXTRACT(code, 31, 30);
    size = 1 << size;

    return size;
}

static uint64_t aarch64_op_offset(uint32_t code)
{
    uint64_t offset;

    offset = BF_EXTRACT(code, 20, 12);
    if ((offset & BIT(8)) != 0) {
        offset |= BITS(63, 9);
    }

    return offset;
}

static errno_t parse_aarch64_ldr(struct insn *insn, uint32_t code)
{
    insn->op.ldr.gpr.dst = (uint8_t)BF_EXTRACT(code, 4, 0);
    insn->op.ldr.gpr.src = (uint8_t)BF_EXTRACT(code, 9, 5);
    insn->op.ldr.offset = aarch64_op_offset(code);
    insn->op.ldr.size = aarch64_op_size(code);

    return SUCCESS;
}

static errno_t parse_aarch64_str(struct insn *insn, uint32_t code)
{
    insn->op.str.gpr.src = (uint8_t)BF_EXTRACT(code, 4, 0);
    insn->op.str.gpr.dst = (uint8_t)BF_EXTRACT(code, 9, 5);
    insn->op.str.offset = aarch64_op_offset(code);
    insn->op.str.size = aarch64_op_size(code);

    return SUCCESS;
}

static errno_t parse_aarch64(struct insn *insn, uint64_t pc)
{
    errno_t ret;
    uint32_t d;
    uint32_t code;
    uint64_t esr;

    code = *(uint32_t *)pc;
    d = code & (BITS(29, 21) | BITS(11, 10));
    esr = insn->vpc->regs[VPC_ESR_EL2];
    if ((esr & ISS_DATA_ABORT_WnR) == 0) {
        insn->type = INSN_TYPE_LDR;
        if (d == OP_LDR_POST) {
            insn->op.ldr.flag.post = 1;
            ret = parse_aarch64_ldr(insn, code);
        } else if (d == OP_LDRS_POST) {
            insn->op.ldr.flag.sign = 1;
            insn->op.ldr.flag.post = 1;
            ret = parse_aarch64_ldr(insn, code);
        } else if (d == OP_LDRS_POST_W) {
            insn->op.ldr.flag.wreg = 1;
            insn->op.ldr.flag.sign = 1;
            insn->op.ldr.flag.post = 1;
            ret = parse_aarch64_ldr(insn, code);
        } else if (d == OP_LDR_WB) {
            insn->op.ldr.flag.wb = 1;
            ret = parse_aarch64_ldr(insn, code);
        } else if (d == OP_LDRS_WB) {
            insn->op.ldr.flag.sign = 1;
            insn->op.ldr.flag.wb = 1;
            ret = parse_aarch64_ldr(insn, code);
        } else if (d == OP_LDRS_WB_W) {
            insn->op.ldr.flag.wreg = 1;
            insn->op.ldr.flag.sign = 1;
            insn->op.ldr.flag.wb = 1;
            ret = parse_aarch64_ldr(insn, code);
        } else {
            ret = -ENOTSUP;
        }
    } else {
        insn->type = INSN_TYPE_STR;
        if (d == OP_STR_POST) {
            insn->op.str.flag.post = 1;
            ret = parse_aarch64_str(insn, code);
        } else if (d == OP_STR_WB) {
            insn->op.str.flag.wb = 1;
            ret = parse_aarch64_str(insn, code);
        } else {
            ret = -ENOTSUP;
        }
    }

    return ret;
}

static errno_t parse_instruction(struct insn *insn)
{
    errno_t ret;
    uint64_t pc;

    ret = vpc_pa_pc(insn->vpc, &pc);
    if (ret == SUCCESS) {
        if (vpc_is_aarch64(insn->vpc)) {
            ret = parse_aarch64(insn, pc);
        } else {
            ret = -ENOTSUP;
        }
    }

    return ret;
}

static errno_t parse_system_register(struct insn *insn)
{
    uint64_t esr;
    uint64_t sas;

    esr = insn->vpc->regs[VPC_ESR_EL2];
    insn->type = ((esr & ISS_DATA_ABORT_WnR) != 0) ? INSN_TYPE_STR : INSN_TYPE_LDR;
    sas = EXTRACT_ISS_DATA_ABORT_SAS(esr);
    insn->op.ldr.size = 1 << sas;

    if (insn->type == INSN_TYPE_LDR) {
        insn->op.ldr.gpr.dst = (uint8_t)EXTRACT_ISS_DATA_ABORT_SRT(esr);
    } else {
        insn->op.ldr.gpr.src = (uint8_t)EXTRACT_ISS_DATA_ABORT_SRT(esr);
    }

    insn->op.ldr.flag.sign = ((esr & ISS_DATA_ABORT_SSE) == 0) ? 0 : 1;
    insn->op.ldr.flag.wreg = ((esr & ISS_DATA_ABORT_SF) == 0) ? 1 : 0;

    return SUCCESS;
}

static errno_t parse(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;
    uint64_t va;
    uint64_t ipa;

    memset(insn, 0, sizeof(*insn));
    insn->vpc = vpc;

    va = vpc->regs[VPC_FAR_EL2];
    ipa = (vpc->regs[VPC_HPFAR_EL2] << 8) | (va & BITS(11, 0));

    insn->op.ldr.va = va;
    insn->op.ldr.ipa = ipa;

    if ((vpc->regs[VPC_ESR_EL2] & ISS_DATA_ABORT_ISV) == 0) {
        ret = parse_instruction(insn);
    } else {
        ret = parse_system_register(insn);
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

