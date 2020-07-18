/*
 * emulator/vgic400/vgic400_default_irq_handler.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void maintenance_misr_eoi(struct vpc *vpc, struct vgic400 *vgic)
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

static errno_t maintenance_interrupt(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_MISR);

    if ((d & BIT(0)) != 0) {    /* issue EOI by EL1 */
        maintenance_misr_eoi(vpc, vgic);
    }

    gic400_eoi(vgic->gic, iar);
    gic400_deactivate(vgic->gic, iar);
    ret = gic400_set_priority_mask(vgic->gic, 0xff);

    return ret;
}

static errno_t el1(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no < 16) {  /* SGI */
        ret = vgic400_inject_sgi(vpc, vgic, iar);
    } else {        /* PPI or SPI */
        ret = vgic400_inject_interrupt(vpc, vgic, iar);
    }

    if (ret == SUCCESS) {
        ret = gic400_set_priority_mask(vgic->gic, 1);
    }

    return ret;
}

static errno_t el2(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    return -ENOTSUP;
}

static errno_t irq_exception(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no == GIC400_MAINTENANCE_INTERRUPT) {
        ret = maintenance_interrupt(vpc, vgic, iar);
    } else if (is_target_irq(vgic, no)) {
        ret = el1(vpc, vgic, iar);
    } else {
        ret = el2(vpc, vgic, iar);
    }

    return ret;
}

errno_t vgic400_default_irq_handler(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;
    uint32_t iar;

    ret = SUCCESS;
    iar = gic400_ack(vgic->gic);
    while (iar != GIC400_SPURIOUS_INTERRUPT) {
        ret = irq_exception(vpc, vgic, iar);
        if (ret != SUCCESS) {
            break;
        }

        iar = gic400_ack(vgic->gic);
    }

    return ret;
}

