/*
 * emulator/vgic400/vgic400_configure.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "driver/system/cpu.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure_gic400(struct gic400 *gic)
{
    uint32_t d;

    d = gic400_read_cpuif(gic, GICC_CTLR);
    d |= BIT(9);    /* GICC_CTLR.EOImodeNS */
    gic400_write_cpuif(gic, GICC_CTLR, d);

    return SUCCESS;
}

static errno_t configure_secondary(struct vgic400 *vgic)
{
    errno_t ret;

    ret = configure_gic400(vgic->gic);

    return ret;
}

static errno_t configure(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;
    uint32_t d;

    memset(vgic, 0, sizeof(*vgic));
    vgic->owner = config->owner;
    vgic->gic = config->gic;
    vgic->base.virtif_control = config->base.virtif_control;
    vgic->base.virtual_cpuif = config->base.virtual_cpuif;
    vgic->priority_mask = gic400_priority_mask(vgic->gic);

    /* probe # of List Register */

    d = gic400_read_virtif_control(vgic, GICH_VTR);
    vgic->nr_list_registers = BF_EXTRACT(d, 5, 0) + 1;

    /* initialize the target map */

    /*   ID  Assign      Source            
     *   31  guest       Legacy IRQ signal              
     *   30  guest       Non-secure Physical timer 
     *   29  N/A         Secure physical timer
     *   28  guest       Legacy FIQ signal
     *   27  guest       Virtual timer
     *   26  hypervisor  Hypervisor timer
     *   25  hypervisor  Virtual maintenance interrupt
     * 24-16 guest       Undefined
     */

    vgic->target.irq[0] = 0x0000d90f;

    ret = configure_gic400(vgic->gic);

    return ret;
}

errno_t vgic400_configure(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;

    if (cpu_no() == 0) {
        if ((vgic != NULL) && (config != NULL)) {
            ret = configure(vgic, config);
        } else {
            ret = -EINVAL;
        }
    } else {
        if ((vgic != NULL) && (config == NULL)) {
            ret = configure_secondary(vgic);
        } else {
            ret = -EINVAL;
        }
    }

    return ret;
}

