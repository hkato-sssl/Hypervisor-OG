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

static void maintenance_misr_eoi(struct vgic400 *vgic, struct vpc *vpc)
{
    uint32_t d;
    uint32_t no;
    uint32_t *iar;

    iar = vgic->iar[vpc->proc_no];
    d = gic400_read_virtif_control(vgic, GICH_EISR0);
    while (d != 0) {
        no = 63 - (uint32_t)aarch64_clz(d);
        gic400_deactivate(vgic->gic, iar[no]);
        gic400_write_virtif_control(vgic, GICH_LR(no), 0);
        d ^= BIT(no);
    }
}

static errno_t maintenance_interrupt(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar)
{
    errno_t ret;
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_MISR);

    if ((d & BIT(0)) != 0) {    /* EOI */
        maintenance_misr_eoi(vgic, vpc);
        d = gic400_read_virtif_control(vgic, GICH_MISR);
    }

    ret = gic400_deactivate(vgic->gic, iar);

    return ret;
}

static errno_t accept_irq(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar)
{
    errno_t ret;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no == GIC400_MAINTENANCE_INTERRUPT) {
        ret = maintenance_interrupt(vgic, vpc, iar);
    } else if (no == GIC400_HYPERVISOR_TIMER) {
        ret = -ENOTSUP;
    } else if (no < 16) {
        ret = vgic400_inject_sgi(vgic, vpc, iar);
    } else if (no < NR_GIC400_INTERRUPTS) {
        /* PPI or SPI */
        ret = vgic400_inject_interrupt(vgic, vpc, no);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t vgic400_emulate_irq_exception(struct vgic400 *vgic, struct vpc *vpc)
{
    errno_t ret;
    uint32_t iar;

    ret = SUCCESS;
    iar = gic400_ack(vgic->gic);
    while (iar != GIC400_SPURIOUS_INTERRUPT) {
        gic400_eoi(vgic->gic, iar);
        ret = accept_irq(vgic, vpc, iar);
        if (ret != SUCCESS) {
            break;
        }

        iar = gic400_ack(vgic->gic);
    }

    return ret;
}

