/*
 * emulator/vgic400/vgic400_inject_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/aarch64.h"
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

static errno_t inject_interrupt(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar, int list_no)
{
    errno_t ret;
    uint32_t d;

    if (iar < 32) {
        d = vgic->ppi[vpc->proc_no].template[iar - 16];
    } else {
        d = vgic->spi.template[iar - 32];
    }

    if (d != 0) {
        gic400_write_virtif_control(vgic, GICH_LR(list_no), d);
        vgic->list[vpc->proc_no].lr[list_no] = d;
        vgic->list[vpc->proc_no].iar[list_no] = iar;
        ret = SUCCESS;
    } else {
        ret = -EPERM;
    }

    return ret;
}

errno_t vgic400_inject_interrupt(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar)
{
    errno_t ret;
    int idx;

    if ((iar > 15) && (iar < NR_GIC400_INTERRUPTS)) {
        idx = vgic400_list_register(vgic);
        if (idx >= 0) {
            ret = inject_interrupt(vgic, vpc, iar, idx);
        } else {
            ret = -EBUSY;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

