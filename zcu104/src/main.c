/*
 * main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/aarch64/system_register.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t init_system(void);
errno_t init_memory_map(void);
void init_hw(void);
void hypervisor_start(void);

int main(void)
{
    init_system();
    init_memory_map();
    init_hw();

    hypervisor_start();

    return 0;
}
