/*
 * memory/memory.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include "lib/system.h"
#include "driver/system/cpu.h"
#include "driver/xilinx/device/mpsoc.h"

/* defines */

/* types */

/* variables */

uint8_t thread_stack_region[CONFIG_NR_CPUS][CONFIG_STACK_SIZE] __attribute__ ((section(".stack.main"), aligned(16)));
uint8_t exc_stack_region[CONFIG_NR_CPUS][CONFIG_EXC_STACK_SIZE] __attribute__ ((section(".stack.exc"), aligned(16)));
uint8_t kernel_stack_region[CONFIG_NR_CPUS][CONFIG_KERNEL_STACK_SIZE] __attribute__ ((section(".stack.kernel"), aligned(16)));
