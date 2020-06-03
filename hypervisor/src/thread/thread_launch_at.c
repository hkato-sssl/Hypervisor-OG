/*
 * thread/thread_launch_at.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64.h"
#include "hypervisor/thread.h"
#include "thread_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t launch_at(uintptr_t *setup, void *entry, void *arg)
{
    setup[THREAD_SETUP_ARG0] = (uintptr_t)arg;
    aarch64_dmb();
    setup[THREAD_SETUP_ENTRY] = (uintptr_t)entry;
    aarch64_dsb();
    aarch64_sev();

    return SUCCESS;
}

errno_t thread_launch_at(uint32_t processor_no, thread_entry_t entry, void *arg)
{
    errno_t ret;
    uintptr_t   *setup;

    setup = thread_setup_array(processor_no);
    if ((setup == NULL) || (entry == NULL)) {
        ret = -EINVAL;
    } else if (setup[THREAD_SETUP_ENTRY] != 0) {
        ret = -EBUSY;
    } else {
        ret = launch_at(setup, entry, arg);
    }

    return ret;
}

