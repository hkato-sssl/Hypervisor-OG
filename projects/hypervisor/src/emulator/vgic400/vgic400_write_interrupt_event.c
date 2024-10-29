// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_write_interrupt_event.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_write_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                                      struct vgic400_interrupt_event *event,
                                      int list_no)
{
    uint32_t d;
    uint32_t n;

    if (event->irq < 16) {
        /* SGI */
        d = vgic->sgi[vpc->proc_no].list_register[event->irq];
        d = (d & ~BITS(17, 10)) | ((uint32_t)(event->cpuid) << 10);
    } else if (event->irq < 32) {
        /* PPI */
        d = vgic->ppi[vpc->proc_no].list_register[event->irq - 16];
    } else {
        /* SPI */
        d = vgic->spi.list_register[event->irq - 32];
    }

    vgic->lr[vpc->proc_no][list_no] = d;
    vgic->iar[vpc->proc_no][list_no] = event->iar;
    gic400_write_virtif_control(vgic, GICH_LR(list_no), d);

    return SUCCESS;
}
