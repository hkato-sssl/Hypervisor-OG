/*
 * system/system_validate_stack_region.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern char __stack_start[];
extern char __stack_end[];

/* functions */

errno_t system_validate_stack_region(const void *addr, size_t size)
{
    errno_t ret;

    if (((uintptr_t)__stack_start <= (uintptr_t)addr) &&
        (((uintptr_t)addr + size) <= (uintptr_t)__stack_end)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

