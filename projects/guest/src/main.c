/*
 * main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/system/cpu.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t init_system(void);
errno_t init_memory_map(void);
void init_hw(void);
void launch_system(void);

int main(void)
{
    init_system();
    init_memory_map();
    init_hw();
    launch_system();

    return 0;
}

__attribute__ ((weak)) void init_hw(void) { return ; }

void launch_system(void)
{
    printk("<%u#%s>\n", cpu_no(), __func__);

    test_vspi_09();
}
