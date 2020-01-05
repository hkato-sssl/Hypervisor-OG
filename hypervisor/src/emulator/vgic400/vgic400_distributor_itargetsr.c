/*
 * emulator/vgic400/vgic400_distributor_itargetsr.c
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

static uint32_t irq_no(uintptr_t reg)
{
    return (uint32_t)(reg - GICD_ITARGETSR(0));
}

static errno_t read_itargetsr_b(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    uint64_t d;
    uintptr_t no;

    no = irq_no(reg);
    if (is_active_irq(vgic, no)) {
        gic400_lock(vgic->gic);
        d = VGIC400_READ8(insn->op.ldr.ipa);
        gic400_unlock(vgic->gic);

        d = vgic400_p2v_cpu_map_b(d, insn->vpc->vm);
    } else {
        d = 0;
    }
    vpc_load_to_gpr_b(insn, d);

    return SUCCESS;
}

static errno_t read_itargetsr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    uint32_t no;
    uint64_t d;
    uint64_t mask;

    no = irq_no(reg);
    mask = vgic400_quad_byte_mask(vgic, no);
    gic400_lock(vgic->gic);
    d = VGIC400_READ32(insn->op.ldr.ipa);
    gic400_unlock(vgic->gic);
    d &= mask;

    d = vgic400_p2v_cpu_map_w(d, insn->vpc->vm);

    vpc_load_to_gpr_w(insn, d);

    return SUCCESS;
}

static errno_t write_itargetsr_b(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    uint64_t d;
    uintptr_t no;

    d = str_value(insn);
    no = irq_no(reg);
    if (is_active_irq(vgic, no)) {
        d = vgic400_v2p_cpu_map_b(d, insn->vpc->vm);

        gic400_lock(vgic->gic);
        VGIC400_WRITE8(insn->op.str.ipa, d);
        gic400_unlock(vgic->gic);
    }

    return SUCCESS;
}

static errno_t write_itargetsr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    uint32_t no;
    uint64_t d;
    uint64_t d0;
    uint64_t mask;

    no = irq_no(reg);
    mask = vgic400_quad_byte_mask(vgic, no);
    d = str_value(insn);
    d &= mask;
    d = vgic400_v2p_cpu_map_w(d, insn->vpc->vm);

    gic400_lock(vgic->gic);
    d0 = VGIC400_READ32(insn->op.str.ipa);
    d |= d0 & ~mask;
    VGIC400_WRITE32(insn->op.str.ipa, d);
    gic400_unlock(vgic->gic);

    return SUCCESS;
}

errno_t vgic400_distributor_itargetsr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (insn->op.ldr.size == 1) {
            ret = read_itargetsr_b(vgic, insn, reg);
        } else if (is_aligned_word_access(insn)) {
            ret = read_itargetsr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (insn->op.str.size == 1) {
            ret = write_itargetsr_b(vgic, insn, reg);
        } else if (is_aligned_word_access(insn)) {
            ret = write_itargetsr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

