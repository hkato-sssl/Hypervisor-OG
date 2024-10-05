/*
 * emulator/vgic400/vgic400_inject_interrupt_event.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint32_t nr_queued_interrupt_events(struct vpc *vpc, struct vgic400 *vgic)
{
    return vgic->interrupt.event_arrays[vpc->proc_no]->num;
}

static errno_t
inject_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                       const struct vgic400_interrupt_event *event, int list_no)
{
    uint32_t d;

    asm volatile ("yield");
    if (event->irq < 16) {
        d = vgic->sgi[vpc->proc_no].list_register[event->irq];
        d = (d & ~BITS(17, 10)) | ((uint32_t)(event->cpuid) << 10);
    } else if (event->irq < 32) {
        d = vgic->ppi[vpc->proc_no].list_register[event->irq - 16];
    } else {
        d = vgic->spi.list_register[event->irq - 32];
    }
    asm volatile ("yield");

    vgic->lr[vpc->proc_no][list_no] = d;
    vgic->iar[vpc->proc_no][list_no] = event->iar;
    gic400_write_virtif_control(vgic, GICH_LR(list_no), d);

    return SUCCESS;
}

errno_t
vgic400_inject_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                               const struct vgic400_interrupt_event *event)
{
    errno_t ret;
    int no;

    vgic400_lock(vgic);

    if (nr_queued_interrupt_events(vpc, vgic) > 0) {
        ret = vgic400_push_interrupt_event(vpc, vgic, event);
    } else {
    no = vgic400_list_register(vgic);
    if (no >= 0) {
            ret = vgic400_write_interrupt_event(vpc, vgic, event, no);
    } else {
            ret = vgic400_push_interrupt_event(vpc, vgic, event);
            if (ret == SUCCESS) {
                ret = -EBUSY; /* all List Regsters are used. */
            }
        }
    }

    vgic400_unlock(vgic);

    return ret;
}
