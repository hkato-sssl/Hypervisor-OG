/*
 * emulator/vgic400/vgic400_distributor_bit_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint64_t irq_no(uintptr_t reg, uintptr_t base)
{
    uint64_t no;

    no = (reg - base) * 8;

    return no;
}

static errno_t read_bit_register_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    errno_t ret;
    uint64_t d;
    uint64_t mask;
    uint64_t no;

    d = VGIC400_READ32(insn->op.ldr.ipa);
    no = irq_no(reg, base);
    mask = vgic->active.irq[no / 32];
    d &= mask;

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static errno_t write_bit_register_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    errno_t ret;
    uint64_t d;
    uint64_t mask;
    uint64_t no;

    d = insn_str_src_value(insn);
    no = irq_no(reg, base);
    mask = vgic->active.irq[no / 32];
    d &= mask;
    VGIC400_WRITE32(insn->op.str.ipa, d);

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t vgic400_distributor_bit_register(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (is_aligned_word_access(insn)) {
            ret = read_bit_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(insn)) {
            ret = write_bit_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

