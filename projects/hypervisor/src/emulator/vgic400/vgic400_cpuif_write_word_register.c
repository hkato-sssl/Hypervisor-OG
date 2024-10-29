// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_cpuif_write_word_register.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t cpuif_write_pmr(const struct insn *insn, struct vgic400 *vgic)
{
    errno_t ret;
    uint32_t d;

    d = (uint32_t)insn_str_src_value(insn);
    d &= vgic->priority_mask;
    gic400_write_virtual_cpuif(vgic, GICV_PMR, d);

    ret = insn_emulate_str(insn);

    return ret;
}

static errno_t cpuif_write_word_register(const struct insn *insn,
                                         struct vgic400 *vgic, uintptr_t reg)
{
    errno_t ret;
    uint32_t d;

    d = insn_str_src_value(insn);
    gic400_write_virtual_cpuif(vgic, reg, (uint32_t)d);

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t vgic400_cpuif_write_word_register(const struct insn *insn,
                                          struct vgic400 *vgic)
{
    errno_t ret;
    uintptr_t reg;

    reg = (uintptr_t)insn->op.str.pa - (uintptr_t)vgic->base.virtual_cpuif;

    if (is_aligned_word_access(insn)) {
        if (reg == GICV_PMR) {
            ret = cpuif_write_pmr(insn, vgic);
        } else if ((reg >= GICV_CTLR) && (reg <= GICV_DIR)) {
            ret = cpuif_write_word_register(insn, vgic, reg);
        } else {
            ret = -EPERM;
        }
    } else {
        ret = -EPERM;
    }

    return ret;
}
