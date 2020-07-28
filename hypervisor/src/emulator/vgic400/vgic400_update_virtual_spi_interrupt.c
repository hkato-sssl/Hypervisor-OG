/*
 * emulator/vgic400emulator/vgic400/vgic400_update_virtual_spi_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/aarch64.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "driver/system/cpu.h"
#include "hypervisor/parameter.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t encourage_virtual_spi_interrupt(struct vgic400 *vgic)
{
    errno_t ret;
    uint8_t priority;
    uint16_t no;
    uint16_t interrupt_no;
    uint32_t d;

    priority = vgic->priority_mask;
    interrupt_no = 0;

    d = vgic->virtual_spi.ipendr & vgic->virtual_spi.ienabler;
    while (d != 0) {
        no = 63 - (uint16_t)aarch64_clz(d);
        if (vgic->virtual_spi.ipriorityr[no] < priority) {
            priority = vgic->virtual_spi.ipriorityr[no];
            interrupt_no = no;
        }
        d &= ~(uint32_t)BIT(no);
    }

    if (priority < vgic->priority_mask) {
        ret = vgic400_inject_virtual_spi_interrupt(vgic, (interrupt_no + vgic->virtual_spi.base_no));
        if (ret == SUCCESS) {
            vgic->virtual_spi.ipendr &= ~(uint32_t)BIT(interrupt_no);
        }
    } else {
        /* No assertable interrupt is pending. */

        ret = SUCCESS;
    }

    return ret;
}

static errno_t send_event(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;
    struct vpc *dst;
    struct vpc_event *event;

    event = &(vgic->vpc_events[vpc->proc_no]);
    if (! event->queued) {
        dst = vpc->vm->vpcs[0];
        event->func = (vpc_event_func_t)vgic400_update_virtual_spi_interrupt;
        event->args[0] = (uintptr_t)dst;
        event->args[1] = (uintptr_t)vgic;
        event->args[2] = 0;
        event->args[3] = 0;
        ret = vpc_send_event(dst, event);
        if (ret == SUCCESS) {
            vgic->virtual_spi.asserting = true;
        }
    } else {
        ret = -EBUSY;
    }

    return ret;
}

errno_t vgic400_update_virtual_spi_interrupt(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;
    uint32_t d;

    vgic400_lock(vgic);

    if (! vgic->virtual_spi.asserting) {
        if (vpc->proc_no == 0) {
            ret = encourage_virtual_spi_interrupt(vgic);
        } else {
            ret = send_event(vpc, vgic);
        }
    } else {
        d = gic400_read_virtif_control(vgic, GICH_LR(0));
        if ((d == 0) && (vpc->proc_no == 0)) {
            ret = encourage_virtual_spi_interrupt(vgic);
        } else {
            ret = SUCCESS;  /* no work */
        }
    }

    vgic400_unlock(vgic);

    return ret;
}

