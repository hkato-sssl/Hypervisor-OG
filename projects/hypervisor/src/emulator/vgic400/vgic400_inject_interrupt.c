/*
 * emulator/vgic400/vgic400_inject_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t inject_interrupt_at(struct vpc *vpc, struct vgic400 *vgic,
                                   uint32_t iar, uint32_t list_no)
{
    errno_t ret;
    uint32_t d;
    uint32_t virq;

    virq = vgic400_irq_to_virq(vgic, iar);

    if (iar < 32) {
        d = vgic->ppi[vpc->proc_no].template[virq - 16];
    } else {
        d = vgic->spi.template[virq - 32];
    }

    if (d != 0) {
        gic400_write_virtif_control(vgic, GICH_LR(list_no), d);
        vgic->lr[vpc->proc_no][list_no] = d;
        vgic->iar[vpc->proc_no][list_no] = iar;
        ret = SUCCESS;
    } else {
        ret = -EPERM;
    }

    return ret;
}

errno_t vgic400_inject_interrupt_at(struct vpc *vpc, struct vgic400 *vgic,
                                    uint32_t iar, uint32_t list_no)
{
    errno_t ret;

    if ((iar < 16) || (iar >= NR_GIC400_INTERRUPTS)) {
        ret = -EINVAL;
    } else if (list_no >= vgic->nr_list_registers) {
        ret = -EINVAL;
    } else {
        ret = inject_interrupt_at(vpc, vgic, iar, list_no);
    }

    return ret;
}

errno_t vgic400_inject_interrupt(struct vpc *vpc, struct vgic400 *vgic,
                                 uint32_t iar)
{
    errno_t ret;
    int idx;

    if ((iar < 16) || (iar >= NR_GIC400_INTERRUPTS)) {
        ret = -EINVAL;
    } else {
        idx = vgic400_list_register(vgic);
        if (idx >= 0) {
            ret = inject_interrupt_at(vpc, vgic, iar, (uint32_t)idx);
        } else {
            ret = -EBUSY;
        }
    }

    return ret;
}
