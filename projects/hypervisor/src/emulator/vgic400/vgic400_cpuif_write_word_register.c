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

static errno_t cpuif_write_word_register(struct vgic400 *vgic,
                                         const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint64_t d;

    d = insn_str_src_value(insn);
    gic400_write_virtual_cpuif(vgic, reg, (uint32_t)d);

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t vgic400_cpuif_write_word_register(struct vgic400 *vgic,
                                          const struct insn *insn,
                                          uintptr_t reg)
{
    errno_t ret;

    if (is_aligned_word_access(insn) && (reg <= GICC_DIR)) {
        ret = cpuif_write_word_register(vgic, insn, reg);
    } else {
        ret = -EPERM;
    }

    return ret;
}
