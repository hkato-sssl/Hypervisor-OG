/*
 * vpc/vpc_send_event.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "driver/system/cpu.h"
#include "hypervisor/parameter.h"
#include "hypervisor/soc.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "vpc_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t assert_event(struct vpc *vpc)
{
    errno_t ret;
    struct soc *soc;

    soc = vpc->vm->soc;
    ret = soc_assert_interrupt_vpc(soc, vpc, HYP_INTR_VPC_EVENT);

    return ret;
}

static errno_t stack_event(struct vpc *vpc, struct vpc_event *event)
{
    event->queued = true;
    event->next = vpc->event_list;
    vpc->event_list = event;

    return SUCCESS;
}

errno_t vpc_send_event(struct vpc *vpc, struct vpc_event *event)
{
    errno_t ret;
    uint32_t lock;

    lock = cpu_lock_interrupts();
    vpc_lock(vpc);

    if (! event->queued) {
        ret = stack_event(vpc, event);
    } else {
        ret = -EBUSY;
    }

    vpc_unlock(vpc);
    cpu_unlock_interrupts(lock);

    if (ret == SUCCESS) {
        ret = assert_event(vpc);
    }

    return ret;
}
