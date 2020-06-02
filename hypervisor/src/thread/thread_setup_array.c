/*
 * thread/thread_setup_array.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "hypervisor/parameter.h"
#include "thread_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static uintptr_t setup_array[HYP_MAX_NR_VPCS][4];

/* functions */

uintptr_t *thread_setup_array(uint32_t no)
{
    uint64_t *p;

    if (no < HYP_MAX_NR_VPCS) {
        p = setup_array[no];
    } else {
        p = NULL;
    }

    return p;
}
