/*
 * emulator/vgic400/vgic400_accept_virtual_spi_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t accept(struct vpc *vpc, struct vgic400 *vgic)
{
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_HCR);
    d |= BIT(3);    /* NPIE: No Pending Interrupt Enable. */
    gic400_write_virtif_control(vgic, GICH_HCR, d);

    vgic->virtual_spi.asserting = true;

    return SUCCESS;
}

static errno_t accept_virtual_spi_interrupt(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;

    vgic400_lock(vgic);
    if (! vgic->virtual_spi.asserting) {
        ret = accept(vpc, vgic);
    } else {
        ret = SUCCESS;  /* no work */
    }
    vgic400_unlock(vgic);

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
        event->func = (vpc_event_func_t)accept_virtual_spi_interrupt;
        event->args[0] = (uintptr_t)dst;
        event->args[1] = (uintptr_t)vgic;
        event->args[2] = 0;
        event->args[3] = 0;
        ret = vpc_send_event(dst, event);
    } else {
        ret = SUCCESS;  /* no work */
    }

    return ret;
}

/* CAUTION:
 * 本APIは内部から呼び出される事を前提としている為、排他制御を実
 * 装していない。
 * 本APIを呼び出す場合は必ずvgic400_lock()を実行している状態で呼
 * び出す必要がある。
 */
errno_t vgic400_accept_virtual_spi_interrupt(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;

    if (! vgic->virtual_spi.asserting) {
        if (vpc->proc_no == 0) {
            ret = accept(vpc, vgic);
        } else {
            ret = send_event(vpc, vgic);
        }
    } else {
        ret = SUCCESS;  /* no work */
    }

    return ret;
}

