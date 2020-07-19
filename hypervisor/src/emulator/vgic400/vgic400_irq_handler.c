/*
 * emulator/vgic400/vgic400_irq_handler.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_irq_handler(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;

    if (vgic->ops->irq_handler != NULL) {
        ret = (*(vgic->ops->irq_handler))(vpc, vgic);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}

