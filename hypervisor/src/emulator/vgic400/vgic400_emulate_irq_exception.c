/*
 * emulator/vgic400/vgic400_emulate_irq_exception.c
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

static errno_t call_interrupt_op(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar, vgic400_interrupt_op_t func)
{
    errno_t ret;

    if (func != NULL) {
        ret = (*func)(vpc, vgic, iar);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}

static errno_t accept_irq(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no == GIC400_MAINTENANCE_INTERRUPT) {
        ret = call_interrupt_op(vpc, vgic, iar, vgic->ops->maintenance);
    } else if (is_target_virq(vgic, no)) {
        if (no < 16) {
            ret = call_interrupt_op(vpc, vgic, iar, vgic->ops->el1.sgi);
        } else if (no < 32) {
            ret = call_interrupt_op(vpc, vgic, iar, vgic->ops->el1.ppi);
        } else {
            ret = call_interrupt_op(vpc, vgic, iar, vgic->ops->el1.spi);
        }
    } else {
        ret = call_interrupt_op(vpc, vgic, iar, vgic->ops->el2);
    }

    return ret;
}

errno_t vgic400_emulate_irq_exception(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;
    uint32_t iar;

    ret = SUCCESS;
    iar = gic400_ack(vgic->gic);
    while (iar != GIC400_SPURIOUS_INTERRUPT) {
        gic400_eoi(vgic->gic, iar);
        ret = accept_irq(vpc, vgic, iar);
        if (ret != SUCCESS) {
            break;
        }

        iar = gic400_ack(vgic->gic);
    }

    return ret;
}

