/*
 * emulator/vgic400/vgic400_activate.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure_interrupt(struct vgic400 *vgic)
{
    errno_t ret;
    struct gic400_interrupt_configuration config;

    memset(&config, 0, sizeof(config));
    config.targets = 1;
    config.priority = 0;
    config.flag.edge = 1;
    ret = gic400_configure_interrupt(vgic->gic, GIC400_MAINTENANCE_INTERRUPT, &config);
    if (ret == SUCCESS) {
        ret = gic400_enable_interrupt(vgic->gic, GIC400_MAINTENANCE_INTERRUPT);
    }

    return ret;
}

errno_t vgic400_activate(struct vgic400 *vgic)
{
    errno_t ret;
    uint32_t d;

    gic400_lock(vgic->gic);
    d = gic400_read_virtif_control(vgic, GICH_HCR);
    d |= BIT(0);    /* GICH_HCR.En */
    gic400_write_virtif_control(vgic, GICH_HCR, d);
    gic400_unlock(vgic->gic);

    ret = configure_interrupt(vgic);

    return ret;
}

