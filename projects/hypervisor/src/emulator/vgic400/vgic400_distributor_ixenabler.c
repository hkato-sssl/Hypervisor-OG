// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_distributor_ixenabler.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_isenabler(struct vgic400 *vgic,
                                      const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ISENABLER(0));

    return ret;
}

errno_t vgic400_distributor_icenabler(struct vgic400 *vgic,
                                      const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ICENABLER(0));

    return ret;
}
