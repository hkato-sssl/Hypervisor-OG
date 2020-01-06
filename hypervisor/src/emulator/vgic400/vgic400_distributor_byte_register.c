/*
 * emulator/vgic400/vgic400_distributor_byte_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
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

    no = reg - base;

    return no;
}

static errno_t read_byte_register_b(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t no;

    no = irq_no(reg, base);
    if (is_active_irq(vgic, no)) {
        gic400_lock(vgic->gic);
        d = VGIC400_READ8(insn->op.ldr.ipa);
        gic400_unlock(vgic->gic);
    } else {
        d = 0;
    }

    vpc_emulate_ldr(insn, d);

    return SUCCESS;
}

static errno_t read_byte_register_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t no;
    uint64_t mask;

    gic400_lock(vgic->gic);
    d = VGIC400_READ32(insn->op.ldr.ipa);
    gic400_unlock(vgic->gic);

    no = irq_no(reg, base);
    mask = vgic400_quad_byte_mask(vgic, no);
    d &= mask;

    vpc_emulate_ldr(insn, d);

    return SUCCESS;
}

static errno_t write_byte_register_b(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t no;

    d = insn_str_src_value(insn);
    no = irq_no(reg, base);
    if (is_active_irq(vgic, no)) {
        gic400_lock(vgic->gic);
        VGIC400_WRITE8(insn->op.str.ipa, d);
        gic400_unlock(vgic->gic);
    }

    return SUCCESS;
}

static errno_t write_byte_register_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t d0;
    uint64_t mask;
    uint64_t no;

    d = insn_str_src_value(insn);
    no = irq_no(reg, base);
    mask = vgic400_quad_byte_mask(vgic, no);
    d &= mask;

    gic400_lock(vgic->gic);
    d0 = VGIC400_READ32(insn->op.str.ipa);
    d |= d0 & ~mask;
    VGIC400_WRITE32(insn->op.str.ipa, d);
    gic400_unlock(vgic->gic);

    return SUCCESS;
}

errno_t vgic400_distributor_byte_register(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (insn->op.ldr.size == 1) {
            ret = read_byte_register_b(vgic, insn, reg, base);
        } else if (is_aligned_word_access(insn)) {
            ret = read_byte_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (insn->op.str.size == 1) {
            ret = write_byte_register_b(vgic, insn, reg, base);
        } else if (is_aligned_word_access(insn)) {
            ret = write_byte_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

