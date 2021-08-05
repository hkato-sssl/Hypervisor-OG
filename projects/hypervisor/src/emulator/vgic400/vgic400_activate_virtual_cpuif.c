/*
 * emulator/vgic400/vgic400_activate_virtual_cpuif.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/parameter.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>
#include <string.h>

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
    config.targets = 0xff; /* This value will be ignored. */
    config.priority = 0;
    config.flag.edge = 1; /* This value will be ignored. */
    ret = gic400_configure_interrupt(vgic->gic, GIC400_MAINTENANCE_INTERRUPT,
                                     &config);
    if (ret == SUCCESS) {
        ret = gic400_enable_interrupt(vgic->gic, GIC400_MAINTENANCE_INTERRUPT);
    }

    if (ret == SUCCESS) {
        ret =
            gic400_configure_interrupt(vgic->gic, HYP_INTR_VPC_EVENT, &config);
        if (ret == SUCCESS) {
            ret = gic400_enable_interrupt(vgic->gic, HYP_INTR_VPC_EVENT);
        }
    }

    return ret;
}

static errno_t call_ops_activate(struct vgic400 *vgic)
{
    errno_t ret;

    if (vgic->ops->activate != NULL) {
        ret = (*(vgic->ops->activate))(vgic);
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t activate_virtual_cpuif(struct vgic400 *vgic)
{
    errno_t ret;
    uint32_t d;

    gic400_lock(vgic->gic);

    ret = call_ops_activate(vgic);
    if (ret == SUCCESS) {
        d = gic400_read_virtif_control(vgic, GICH_HCR);
        d |= BIT(0); /* GICH_HCR.En */
        gic400_write_virtif_control(vgic, GICH_HCR, d);
    }

    gic400_unlock(vgic->gic);

    return ret;
}

errno_t vgic400_activate_virtual_cpuif(struct vgic400 *vgic)
{
    errno_t ret;

    ret = activate_virtual_cpuif(vgic);
    if (ret == SUCCESS) {
        ret = configure_interrupt(vgic);
    }

    return ret;
}
