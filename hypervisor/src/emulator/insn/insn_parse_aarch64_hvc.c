/*
 * emulate/insn/insn_parse_aarch64_hvc.c
 *
 * (C) 2020 Hidekazu Kato
 *
 * CAUTION:
 * 本関数はエミュレーション処理の過程で呼ばれる事を想定している。
 * その為、PCのアライメント違反状態では本関数が呼び出される事は
 * 無いのでアライメントチェックは実行しない。
 */

#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t parse_aarch64_hvc(struct insn *insn, struct vpc *vpc, uint32_t code)
{
    errno_t ret;
    uint32_t mask;

    mask = BITS(31, 21) | BITS(4, 0);
    if ((code & mask) == 0xd4000002) {
        insn->type = INSN_TYPE_HVC;
        insn->op.hvc.imm = (uint16_t)BF_EXTRACT(code, 20, 5);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t insn_parse_aarch64_hvc(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;
    uint64_t pc;
    uint32_t code;

    memset(insn, 0, sizeof(*insn));

    pc = vpc->regs[VPC_PC] - 4;
    ret = vpc_read_instruction(vpc, &code, pc);
    if (ret == SUCCESS) {
        ret = parse_aarch64_hvc(insn, vpc, code);
    }

    return ret;
}

