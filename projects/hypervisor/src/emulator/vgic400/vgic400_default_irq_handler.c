/*
 * emulator/vgic400/vgic400_default_irq_handler.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/system_register.h"
#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/system/cpu.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/parameter.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/aarch64.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "vgic400_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void enable_underflow_interrupt(struct vgic400 *vgic)
{
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_HCR);
    d |= BIT(1); /* GICH_HCR.UIE */
    gic400_write_virtif_control(vgic, GICH_HCR, d);
}

static void disable_underflow_interrupt(struct vgic400 *vgic)
{
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_HCR);
    d &= ~BIT(1); /* GICH_HCR.UIE */
    gic400_write_virtif_control(vgic, GICH_HCR, d);
}

static void maintenance_misr_eoi(struct vpc *vpc, struct vgic400 *vgic)
{
    uint32_t no;
    uint32_t iar;
    uint32_t irq;
    uint64_t d;

    d = gic400_read_virtif_control(vgic, GICH_EISR0);
    while (d != 0) {
        no = 63 - (uint32_t)aarch64_clz(d);
        gic400_write_virtif_control(vgic, GICH_LR(no), 0);

        iar = vgic->iar[vpc->proc_no][no];
        irq = BF_EXTRACT(iar, 9, 0);
        if (irq < NR_GIC400_INTERRUPTS) {
            gic400_deactivate(vgic->gic, iar);
        }

        d &= ~BIT(no);
    }
}

static errno_t maintenance_interrupt_underflow(struct vpc *vpc,
                                               struct vgic400 *vgic)
{
    errno_t ret;
    struct vgic400_interrupt_event event;

    ret = vgic400_pop_interrupt_event(vpc, vgic, &event);
    if (ret == SUCCESS) {
        ret = vgic400_inject_interrupt_event(vpc, vgic, &event);
    } else if (ret == -ENODATA) {
        disable_underflow_interrupt(vgic);
        ret = SUCCESS;
    } else {
        printk("%s: vgic400_pop_interrupt_event() -> %d\n", __func__, ret);
    }

    return ret;
}

static errno_t maintenance_interrupt(struct vpc *vpc, struct vgic400 *vgic,
                                     uint32_t iar)
{
    errno_t ret;
    uint32_t d;

    ret = SUCCESS;
    d = gic400_read_virtif_control(vgic, GICH_MISR);

    if ((d & BIT(0)) != 0) { /* EOI has been issued by EL1 */
        maintenance_misr_eoi(vpc, vgic);
    }

    if ((d & BIT(1)) != 0) { /* Undeflow */
        ret = maintenance_interrupt_underflow(vpc, vgic);
    }

    ret = gic400_eoi_and_deactivate(vgic->gic, iar);

    return ret;
}

static errno_t el1(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    struct vgic400_interrupt_event event;
    struct vgic400_interrupt_event_array *array;

    gic400_eoi(vgic->gic, iar);

    ret = vgic400_create_interrupt_event(vpc, vgic, &event, iar);
    if (ret != SUCCESS) {
        printk("%s: An illegal interrupt has been issued.\n", __func__);
        return ret;
    }

    array = vgic->interrupt.event_arrays[vpc->proc_no];
    spin_lock(&(array->lock));

    if (array->num > 0) {
        ret = vgic400_push_interrupt_event(vpc, vgic, &event);
    } else {
        ret = vgic400_inject_interrupt_event(vpc, vgic, &event);
        if (ret == -EBUSY) {
            ret = vgic400_push_interrupt_event(vpc, vgic, &event);
            enable_underflow_interrupt(vgic);
        }
    }

    spin_unlock(&(array->lock));

    return ret;
}

static errno_t receive_vpc_event(struct vpc *vpc, struct vgic400 *vgic,
                                 uint32_t iar)
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

static errno_t irq_exception(struct vpc *vpc, struct vgic400 *vgic,
                             uint32_t iar)
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
