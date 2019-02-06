/*
 * config/memory.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

uint8_t main_stack_region[NR_CPUS][STACK_SIZE] __attribute__ ((section(".stack.main"), aligned(16)));
uint8_t exc_stack_region[NR_CPUS][STACK_SIZE] __attribute__ ((section(".stack.exc"), aligned(16)));

