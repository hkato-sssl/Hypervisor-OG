/*
 * emulator/vgic400/vgic400_push_interrupt_event.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t push_interrupt_event(struct vgic400_interrupt_event_array *array,
                                    const struct vgic400_interrupt_event *event)
{
    errno_t ret;
    uint32_t parent;
    uint32_t i;
    uint32_t d;

    spin_lock(&(array->lock));

    if (array->num == NR_VGIC400_INTERRUPT_EVENTS) {
        ret = -ENOMEM;
        goto exit_push;
    }

    d = vgic400_encode_interrupt_event(event);
    i = array->num;
    parent = (i - 1) / 2;
    while ((i > 0) && (d < array->events[parent])) {
        array->events[i] = array->events[parent];
        i = parent;
        parent = (i - 1) / 2;
    }
    array->events[i] = d;

    ++(array->num);
    ret = SUCCESS;

exit_push:
    spin_unlock(&(array->lock));

    return ret;
}

errno_t
vgic400_push_interrupt_event(struct vgic400 *vgic, uint32_t proc_no,
                             const struct vgic400_interrupt_event *event)
{
    errno_t ret;

    if ((proc_no < NR_VGIC400_CPUS)
        && (vgic->interrupt.event_arrays[proc_no] != NULL)) {
        ret =
            push_interrupt_event(vgic->interrupt.event_arrays[proc_no], event);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
