/*
 * hypervisor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "driver/system/cpu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void hypervisor(void)
{
    printk("CPU#%u\n", cpu_no());
    for (;;);
}

