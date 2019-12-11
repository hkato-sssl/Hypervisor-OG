/*
 * main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"

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

void init_hw(void) { return ; }
void launch_system(void)
{
    printk("Done\n");
}