/*
 * init/init_hw.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/system/cpu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

struct gic400 gic;

/* functions */

void init_hw(void)
{
    errno_t ret;
    struct gic400_configuration config;

    if (cpu_no() == 0) {
        memset(&config, 0, sizeof(config));
        config.base.distributor = (void *)CONFIG_GICD_BASE;
        config.base.cpuif = (void *)CONFIG_GICC_BASE;
        ret = gic400_init(&gic, &config);
    } else {
        ret = gic400_init(&gic, NULL);
    }

    if (ret != SUCCESS) {
        printk("gic400_configure() -> %d", ret);
    }
}
