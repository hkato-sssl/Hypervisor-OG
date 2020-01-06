/*
 * emulator/vgic400/vgic400_distributor_icfgr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint64_t byte_mask(const struct vgic400 *vgic, uintptr_t reg)
{
    uint32_t mask;
    uint32_t no;

    static uint8_t table[16] = {
        0x00, 0x03, 0x0c, 0x0f,
        0x30, 0x33, 0x3c, 0x3f,
        0xc0, 0xc3, 0xcc, 0xcf,
        0xf0, 0xf3, 0xfc, 0xff
    };

    no = (uint32_t)(reg - GICD_ICFGR(0)) * 4;   /* interrupt No. in LSB */
    mask = vgic->active.irq[no / 32];
    mask = (mask >> (no % 32)) & 0x0f;
    mask = table[mask];

    return mask;
}

static uint64_t word_mask(const struct vgic400 *vgic, uintptr_t reg)
{
    uint64_t mask;
    uint64_t m0, m1, m2, m3;

    m0 = byte_mask(vgic, reg);
    m1 = byte_mask(vgic, (reg + 1));
    m2 = byte_mask(vgic, (reg + 2));
    m3 = byte_mask(vgic, (reg + 3));
    mask = (m3 << 24) | (m2 << 16) | (m1 << 8) | m0;

    return mask;
}

static errno_t read_icfgr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    uint64_t d;
    uint64_t mask;

    mask = word_mask(vgic, reg);
    gic400_lock(vgic->gic);
    d = VGIC400_READ32(insn->op.ldr.ipa);
    gic400_unlock(vgic->gic);
    d &= mask;

    vpc_emulate_ldr(insn, d);

    return SUCCESS;
}

static errno_t write_icfgr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    uint64_t d;
    uint64_t d0;
    uint64_t mask;

    d = insn_str_src_value(insn);
    mask = word_mask(vgic, reg);
    d &= mask;

    gic400_lock(vgic->gic);
    d0 = VGIC400_READ32(insn->op.str.ipa);
    d |= (d0 & ~mask);
    VGIC400_WRITE32(insn->op.str.ipa, d);
    gic400_unlock(vgic->gic);

    return SUCCESS;
}

errno_t vgic400_distributor_icfgr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (is_aligned_word_access(insn)) {
            ret = read_icfgr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(insn)) {
            ret = write_icfgr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

