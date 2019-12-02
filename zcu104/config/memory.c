/*
 * config/memory.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include "lib/system.h"

/* defines */

/* types */

/* variables */

uint8_t main_stack_region[CONFIG_NR_CPUS][CONFIG_STACK_SIZE] __attribute__ ((section(".stack.main"), aligned(16)));
uint8_t exc_stack_region[CONFIG_NR_CPUS][CONFIG_EXC_STACK_SIZE] __attribute__ ((section(".stack.exc"), aligned(16)));

void *exc_stack_top(void)
{
    uint8_t *p;
    uint32_t no;

    no = cpu_no();
    p = exc_stack_region[no] + CONFIG_EXC_STACK_SIZE;

    return p;
}
