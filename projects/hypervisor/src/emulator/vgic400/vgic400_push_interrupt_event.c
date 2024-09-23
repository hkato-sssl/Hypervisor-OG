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

static errno_t push(struct vgic400_interrupt_event_array *array, uint64_t event)
{
    uint32_t i;
    uint32_t parent;

    i = array->num;
    parent = (i - 1) / 2;
    while ((i > 0) && (is_less(event, array->events[parent]))) {
        array->events[i] = array->events[parent];
        i = parent;
        parent = (i - 1) / 2;
    }
    array->events[i] = event;

    ++(array->num);

    return SUCCESS;
}

static errno_t push_interrupt_event(struct vgic400_interrupt_event_array *array,
                                    uint64_t event)
{
    errno_t ret;

    spin_lock(&(array->lock));

    if (array->num < NR_VGIC400_INTERRUPT_EVENTS) {
        ret = push(array, event);
    } else {
        ret = -ENOMEM;
    }

    spin_unlock(&(array->lock));

    return ret;
}

errno_t
vgic400_push_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                             const struct vgic400_interrupt_event *event)
{
    errno_t ret;
    uint64_t d;
    struct vgic400_interrupt_event_array *array;

    array = vgic->interrupt.event_arrays[vpc->proc_no];
    if (array != NULL) {
        d = vgic400_encode_interrupt_event(event);
        ret = push_interrupt_event(array, d);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
