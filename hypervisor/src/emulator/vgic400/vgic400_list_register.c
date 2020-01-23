/*
 * emulator/vgic400/vgic400_list_register.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/aarch64.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

int vgic400_list_register(struct vgic400 *vgic)
{
    int no;
    uint32_t d;
    uint32_t ct;

    d = gic400_read_virtif_control(vgic, GICH_ELSR0);
    ct = (uint32_t)aarch64_clz(d);
    if (ct < 64) {
        no = 63 - (int)ct;
    } else {
        no = -1;
    }

    return no;
}

