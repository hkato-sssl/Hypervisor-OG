/*
 * thread/thread_parameter_array.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "hypervisor/parameter.h"
#include "hypervisor/thread.h"
#include "thread_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static uintptr_t parameter_array[HYP_MAX_NR_VPCS][NR_THREAD_PARAMETERS];

/* functions */

uintptr_t *thread_parameter_array(uint32_t no)
{
    uint64_t *p;

    if (no < HYP_MAX_NR_VPCS) {
        p = parameter_array[no];
    } else {
        p = NULL;
    }

    return p;
}

