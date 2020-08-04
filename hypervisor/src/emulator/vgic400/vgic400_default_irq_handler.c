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
#include "hypervisor/parameter.h"
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
    uint64_t d;
    uint32_t no;
    uint32_t *iar;

    iar = vgic->iar[vpc->proc_no];
    d = gic400_read_virtif_control(vgic, GICH_EISR0);

    while (d != 0) {
        no = 63 - (uint32_t)aarch64_clz(d);
        gic400_write_virtif_control(vgic, GICH_LR(no), 0);
        if ((! vgic->boolean.virtual_spi) || (no > 0)) {
            gic400_deactivate(vgic->gic, iar[no]);
        }
        d ^= BIT(no);
    }
}

static errno_t maintenance_interrupt(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_MISR);

    if ((d & BIT(0)) != 0) {    /* EOI has issued by EL1 */
        maintenance_misr_eoi(vpc, vgic);
    }

    if ((d & BIT(1)) != 0) {    /* Undeflow */
        d = gic400_read_virtif_control(vgic, GICH_HCR);
        d ^= BIT(1);    /* GICH_HCR.UIE */
        gic400_write_virtif_control(vgic, GICH_HCR, d);
        /* Enable EL1 interrupts */
        gic400_set_priority_mask(vgic->gic, 0xff);
    }

    if ((d & BIT(3)) != 0) {     /* No pending */
        vgic400_expose_virtual_spi_interrupt(vpc, vgic);
    }

    ret = gic400_eoi_and_deactivate(vgic->gic, iar);

    return ret;
}

static errno_t el1(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t d;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no < 16) {  /* SGI */
        ret = vgic400_inject_sgi(vpc, vgic, iar);
    } else {        /* PPI or SPI */
        ret = vgic400_inject_interrupt(vpc, vgic, iar);
    }

    if (ret == SUCCESS) {
        gic400_eoi(vgic->gic, iar);
        d = gic400_read_virtif_control(vgic, GICH_ELRSR0);
        if (vgic->boolean.virtual_spi) {
            d &= ~(uint32_t)BIT(0); /* Bit-0 is reserved for virtual SPI. */
        }
        if (d == 0) {
            d = gic400_read_virtif_control(vgic, GICH_HCR);
            d |= BIT(1);    /* GICH_HCR.UIE */
            gic400_write_virtif_control(vgic, GICH_HCR, d);
            /* Disable EL1 interrupts */
            ret = gic400_set_priority_mask(vgic->gic, 1);
        }
    }

    return ret;
}

static errno_t receive_vpc_event(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;

    vpc_call_event(vpc);

    ret = gic400_eoi_and_deactivate(vgic->gic, iar);

    return ret;
}

static errno_t el2(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t no;

    no = BF_EXTRACT(iar, 9, 0);
    if (no == HYP_INTR_VPC_EVENT) {
        ret = receive_vpc_event(vpc, vgic, iar);
    } else {
        if (vgic->ops->el2_irq_handler != NULL) {
            ret = (*(vgic->ops->el2_irq_handler))(vpc, vgic, iar);
        } else {
            ret = -ENOTSUP;
        }
    }

    return ret;
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

