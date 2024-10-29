// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_cpuif_emulate_memory_insn.c
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
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_cpuif_emulate_memory_access(const struct insn *insn,
                                            struct vgic400 *vgic)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_STR) {
        ret = vgic400_cpuif_write_word_register(insn, vgic);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}
