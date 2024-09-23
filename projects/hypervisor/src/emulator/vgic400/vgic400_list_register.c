/*
 * emulator/vgic400/vgic400_list_register.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/aarch64.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

int vgic400_list_register(struct vgic400 *vgic)
{
    int no;
    uint64_t d;

    d = gic400_read_virtif_control(vgic, GICH_ELRSR0);
    no = 63 - (int)aarch64_clz(d); /* -1: No List Register is available. */

    return no;
}
