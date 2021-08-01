/*
 * emulator/vgic400/vgic400_cpuif_emulate_memory_insn.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t emulate_memory_insn(const struct insn *insn, struct vgic400 *vgic)
{
    errno_t ret;
    uintptr_t base;
    uintptr_t reg;

    base = (uintptr_t)vgic->base.virtual_cpuif;
    reg = (insn->op.str.pa - base);
    if (reg == GICC_PMR) {
        ret = vgic400_cpuif_write_pmr(vgic, insn);
    } else {
        ret = vgic400_cpuif_write_word_register(vgic, insn, reg);
    }

    return ret;
}

errno_t vgic400_cpuif_emulate_memory_access(const struct insn *insn, struct vgic400 *vgic)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_STR) {
        ret = emulate_memory_insn(insn, vgic);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}

