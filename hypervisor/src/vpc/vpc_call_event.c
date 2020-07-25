/*
 * vpc/vpc_call_event.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/memio.h"
#include "driver/system/cpu.h"
#include "hypervisor/vpc.h"
#include "vpc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static struct vpc_event *get_event(struct vpc *vpc)
{
    struct vpc_event *event;
    uint32_t lock;

    lock = cpu_lock_interrupts();
    vpc_lock(vpc);

    event = vpc->event_list;
    vpc->event_list = NULL;

    vpc_unlock(vpc);
    cpu_unlock_interrupts(lock);

    return event;
}

void vpc_call_event(struct vpc *vpc)
{
    struct vpc_event *event;
    struct vpc_event *next;

    event = get_event(vpc);
    while (event != NULL) {
        (*(event->func))(event->args[0], event->args[1], event->args[2], event->args[3]); 
        next = event->next;
        event->next = NULL;
        event->func = NULL;
        memory_barrier();
        event->queued = false;
        memory_barrier();

        event = next;
    }
}
