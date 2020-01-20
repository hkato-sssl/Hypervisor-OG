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

static int list_register(struct vgic400 *vgic)
{
    int no;
    uint32_t d;
    uint32_t ct;

    d = gic400_read_virtif_control(vgic, GICH_ELSR0);
    ct = (uint32_t)aarch64_clz(d);
    if (ct < 32) {
        no = 31 - (int)ct;
    } else {
        no = -1;
    }

    return no;
}

static errno_t inject_interrupt(struct vgic400 *vgic, uint32_t no, int idx)
{
    errno_t ret;
    uint32_t d;

    if (no < 32) {
        d = vgic->template.ppi[no - 15];
    } else {
        d = vgic->template.spi[no - 32];
    }

    if (d != 0) {
        vgic->list[idx] = d;
        gic400_write_virtif_control(vgic, GICH_LR(idx), d);
        ret = SUCCESS;
    } else {
        ret = -EPERM;
    }

    return ret;
}

errno_t vgic400_inject_interrupt(struct vgic400 *vgic, struct vpc *vpc, uint32_t no)
{
    errno_t ret;
    int idx;

    if ((no > 15) && (no < NR_GIC400_INTERRUPTS)) {
        gic400_lock(vgic->gic);
        idx = list_register(vgic);
        if (idx >= 0) {
            ret = inject_interrupt(vgic, no, idx);
        } else {
            ret = -EBUSY;
        }
        gic400_unlock(vgic->gic);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

