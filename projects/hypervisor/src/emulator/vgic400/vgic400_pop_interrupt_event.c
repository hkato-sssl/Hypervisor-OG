// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_pop_interrupt_event.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

static void insert_event(struct vgic400_interrupt_event_array *array,
                         uint64_t event, uint32_t i);

/* variables */

/* functions */

static void swap_event(struct vgic400_interrupt_event_array *array,
                       uint64_t event, uint32_t i, uint32_t child)
{
    uint64_t d;

    d = array->events[child];
    if (is_less(d, event)) {
        array->events[i] = d;
        insert_event(array, event, child);
    } else {
        array->events[i] = event;
    }
}

static void insert_event(struct vgic400_interrupt_event_array *array,
                         uint64_t event, uint32_t i)
{
    uint32_t child;

    child = i * 2 + 1;
    if (child < array->num) {
        if ((child + 1) < array->num) {
            if (is_less(array->events[child], array->events[child + 1])) {
                swap_event(array, event, i, child);
            } else {
                swap_event(array, event, i, (child + 1));
            }
        } else {
            swap_event(array, event, i, child);
        }
    } else {
        array->events[i] = event;
    }
}

static errno_t pop(struct vgic400_interrupt_event_array *array,
                   struct vgic400_interrupt_event *event)
{
    vgic400_decode_interrupt_event(event, array->events[0]);

    --(array->num);
    if (array->num > 0) {
        insert_event(array, array->events[array->num], 0);
    }

    return SUCCESS;
}

static errno_t pop_interrupt_event(struct vgic400_interrupt_event_array *array,
                                   struct vgic400_interrupt_event *event)
{
    errno_t ret;

    spin_lock(&(array->lock));

    if (array->num > 0) {
        ret = pop(array, event);
    } else {
        ret = -ENODATA;
    }

    spin_unlock(&(array->lock));

    return ret;
}

errno_t vgic400_pop_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                                    struct vgic400_interrupt_event *event)
{
    errno_t ret;
    struct vgic400_interrupt_event_array *array;

    array = vgic->interrupt.event_arrays[vpc->proc_no];
    if (array != NULL) {
        ret = pop_interrupt_event(array, event);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
