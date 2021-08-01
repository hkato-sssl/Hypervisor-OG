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

    vgic400_lock(vgic);

    d = gic400_read_virtif_control(vgic, GICH_ELRSR0);
    if (vgic->boolean.virtual_spi) {
        d &= ~(uint32_t)BIT(0); /* bit-0 is reserved for Virtual SPI */
    }
    ct = (uint32_t)aarch64_clz(d);
    no = 63 - (int)ct;          /* -1: No List Register is available. */

    vgic400_unlock(vgic);

    return no;
}

