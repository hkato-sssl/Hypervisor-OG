/*
 * emulator/vgic400/vgic400_inject_sgi.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
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

static errno_t inject_sgi(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar, int list_no)
{
    uint32_t d;
    uint32_t id;
    uint32_t src;

    id = BF_EXTRACT(iar, 9, 0);
    vgic->sgi.iar[id] = iar;

    src = BF_EXTRACT(iar, 12, 10);
    src = vpc->vm->proc_map.virtual[src];

    d = BIT(28) | BIT(19);                              /* State = pending, EOI */
    d |= vgic->sgi.priority[vpc->proc_no][id] << 20;    /* priority */
    d |= src << 10;                                     /* virtual CPUID */
    d |= id;                                            /* Interrupt ID */

    gic400_write_virtif_control(vgic, GICH_LR(list_no), d);

    return SUCCESS;
}

errno_t vgic400_inject_sgi(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar)
{
    errno_t ret;
    int idx;

    gic400_lock(vgic->gic);

    idx = vgic400_list_register(vgic);
    if (idx >= 0) {
        ret = inject_sgi(vgic, vpc, iar, idx);
    }

    gic400_unlock(vgic->gic);

    return ret;
}

