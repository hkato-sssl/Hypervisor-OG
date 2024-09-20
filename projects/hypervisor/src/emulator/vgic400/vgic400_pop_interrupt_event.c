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
                         uint32_t event, uint32_t i);

/* variables */

/* functions */

static void swap_event(struct vgic400_interrupt_event_array *array,
                       uint32_t event, uint32_t i, uint32_t child)
{
    uint32_t d;

    d = array->events[child];
    if (d < event) {
        array->events[i] = d;
        insert_event(array, event, child);
    } else {
        array->events[i] = event;
    }
}

static void insert_event(struct vgic400_interrupt_event_array *array,
                         uint32_t event, uint32_t i)
{
    uint32_t child;

    child = i * 2 + 1;
    if (child < array->num) {
        if ((child + 1) < array->num) {
            if (array->events[child] < array->events[child + 1]) {
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

static errno_t pop_interrupt_event(struct vgic400_interrupt_event_array *array,
                                   struct vgic400_interrupt_event *event)
{
    errno_t ret;
    uint32_t d;

    spin_lock(&(array->lock));

    if (array->num > 0) {
        d = array->events[0];
        vgic400_decode_interrupt_event(event, d);

        --(array->num);
        if (array->num > 0) {
            insert_event(array, array->events[array->num], 0);
        }
        ret = SUCCESS;
    } else {
        ret = -EIO;
    }

    spin_unlock(&(array->lock));

    return ret;
}

errno_t vgic400_pop_interrupt_event(struct vgic400 *vgic,
                                    struct vgic400_interrupt_event *event,
                                    uint32_t proc_no)
{
    errno_t ret;

    if ((proc_no < NR_VGIC400_CPUS)
        && (vgic->interrupt.event_arrays[proc_no] != NULL)) {
        ret = pop_interrupt_event(vgic->interrupt.event_arrays[proc_no], event);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
