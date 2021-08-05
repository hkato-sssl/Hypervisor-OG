/*
 * emulator/vgic400/vgic400_inject_sgi.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t inject_sgi_at(struct vpc *vpc, struct vgic400 *vgic,
                             uint32_t iar, uint32_t list_no)
{
    uint32_t d;
    uint32_t id;
    uint32_t src;

    id = BF_EXTRACT(iar, 9, 0);
    src = BF_EXTRACT(iar, 12, 10);
    src = vpc->vm->proc_map.virtual[src];

    d = BIT(28) | BIT(19);                           /* State = pending, EOI */
    d |= vgic->sgi[vpc->proc_no].priority[id] << 20; /* priority */
    d |= src << 10;                                  /* virtual CPUID */
    d |= id;                                         /* Interrupt ID */

    gic400_write_virtif_control(vgic, GICH_LR(list_no), d);
    vgic->lr[vpc->proc_no][list_no] = d;
    vgic->iar[vpc->proc_no][list_no] = iar;

    return SUCCESS;
}

errno_t vgic400_inject_sgi_at(struct vpc *vpc, struct vgic400 *vgic,
                              uint32_t iar, uint32_t list_no)
{
    errno_t ret;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no >= 16) {
        ret = -EINVAL;
    } else if (list_no >= vgic->nr_list_registers) {
        ret = -EINVAL;
    } else {
        ret = inject_sgi_at(vpc, vgic, iar, list_no);
    }

    return ret;
}

errno_t vgic400_inject_sgi(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    int idx;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no >= 16) {
        ret = -EINVAL;
    } else {
        idx = vgic400_list_register(vgic);
        if (idx >= 0) {
            ret = inject_sgi_at(vpc, vgic, iar, (uint32_t)idx);
        } else {
            ret = -EBUSY;
        }
    }

    return ret;
}
