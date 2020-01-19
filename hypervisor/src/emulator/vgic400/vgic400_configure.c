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
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;
    uint16_t i;
    uint16_t no;
    uint32_t bit;

    memset(vgic, 0, sizeof(*vgic));
    vgic->owner = config->owner;
    vgic->gic = config->gic;

    vgic->target.irq[0] = 0xffff00ff;
    ret = SUCCESS;
    for (i = 0; i < config->irq.num; ++i) {
        no = config->irq.array[i];
        if ((no >= 32) && (no < NR_GIC400_INTERRUPTS)) {
            bit = 1 << (no & 31);
            vgic->target.irq[no / 32] |= bit;
        } else {
            ret = -EINVAL;
            break;
        }
    }

    return ret;
}

errno_t vgic400_configure(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;

    if ((vgic != NULL) && (config != NULL)) {
        ret = configure(vgic, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

