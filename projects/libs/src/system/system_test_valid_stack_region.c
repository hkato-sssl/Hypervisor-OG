/*
 * system/system_test_valid_stack_region.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/system.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

extern char __stack_start[];
extern char __stack_end[];

/* functions */

bool system_test_valid_stack_region(const void *addr, size_t size)
{
    bool test;

    if (((uintptr_t)__stack_start <= (uintptr_t)addr)
        && (((uintptr_t)addr + size) <= (uintptr_t)__stack_end)) {
        test = true;
    } else {
        test = false;
    }

    return test;
}
