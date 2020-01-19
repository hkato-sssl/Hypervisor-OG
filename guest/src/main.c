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
    uint64_t d;

    printk("<%s>\n", __func__);

    for (d = 1; d != 0; d <<= 1) {
        printk("cls(%p)=%u\n", d, aarch64_cls(d));
        printk("clz(%p)=%u\n", d, aarch64_clz(d));
        printk("\n");
    }
}
