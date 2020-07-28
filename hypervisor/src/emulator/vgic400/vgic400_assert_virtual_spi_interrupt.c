/*
 * emulator/vgic400emulator/vgic400_assert_virtual_spi_interrupt.c
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

static errno_t send_event(struct vpc *vpc, struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;
    struct vpc *dst;
    struct vpc_event *event;

    event = &(vgic->vpc_events[vpc->proc_no]);
    if (! event->queued) {
        dst = vpc->vm->vpcs[0];
        event->func = (vpc_event_func_t)vgic400_assert_virtual_spi_interrupt;
        event->args[0] = (uintptr_t)dst;
        event->args[1] = (uintptr_t)vgic;
        event->args[2] = interrupt_no;
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

static errno_t assert_virtual_spi_interrupt(struct vpc *vpc, struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;

    if (vpc->proc_no == 0) {
        ret = vgic400_inject_virtual_spi_interrupt(vgic, interrupt_no);
    } else {
        ret = send_event(vpc, vgic, interrupt_no);
    }

    return ret;
}

static errno_t validate_parameters(struct vpc *vpc, struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;
    uint32_t base;

    if (vgic->boolean.virtual_spi) {
        base = vgic->virtual_spi.base_no;
        if ((interrupt_no >= base) && (interrupt_no < (base + 32))) {
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EPERM;
    }

    return ret;
}

errno_t vgic400_assert_virtual_spi_interrupt(struct vpc *vpc, struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;

    ret = validate_parameters(vpc, vgic, interrupt_no);
    if (ret == SUCCESS) {
        vgic400_lock(vgic);
        if (! vgic->virtual_spi.asserting) {
            ret = assert_virtual_spi_interrupt(vpc, vgic, interrupt_no);
        } else {
            /* Only update PENDR */
            vgic->virtual_spi.ipendr |= 1UL << (interrupt_no - vgic->virtual_spi.base_no);
        }
        vgic400_unlock(vgic);
    }

    return ret;
}

