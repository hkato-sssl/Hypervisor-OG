/*
 * emulator/vgic400/vgic400_cpuif_write_pmr.c
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

static errno_t cpuif_write_pmr(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;
    uint32_t d;

    d = (uint32_t)insn_str_src_value(insn);
    d &= vgic->priority_mask;
    gic400_write_virtual_cpuif(vgic, GICV_PMR, d);

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t vgic400_cpuif_write_pmr(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;

    if (is_aligned_word_access(insn)) {
        ret = cpuif_write_pmr(vgic, insn);
    } else {
        ret = -EPERM;
    }

    return ret;
}
