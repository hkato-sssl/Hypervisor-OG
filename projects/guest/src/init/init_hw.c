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
#include "driver/aarch64/psci.h"
#include "driver/system/cpu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

struct gic400 gic;

/* functions */

static void wakeup_secondary_processors(void)
{
    int i;
    errno_t ret;
    void _start(void);

    for (i = 1; i < CONFIG_NR_CPUS; ++i) {
        ret = aarch64_psci_cpu_on(i, _start, 0);
        if (ret != SUCCESS) {
            printk("aarch64_psci_cpu_on(%d) -> %d\n", i, ret);
        }
    }
}

static errno_t init_gic400(void)
{
    errno_t ret;
    struct gic400_configuration config;

    if (cpu_no() == 0) {
        memset(&config, 0, sizeof(config));
        config.base.distributor = (void *)CONFIG_GICD_BASE;
        config.base.cpuif = (void *)CONFIG_GICC_BASE;
        ret = gic400_initialize(&gic, &config);
    } else {
        ret = gic400_initialize(&gic, NULL);
    }

    if (ret != SUCCESS) {
        printk("gic400_initialize() -> %d", ret);
    }

    return ret;
}

static void init_primary_processor(void)
{
    errno_t ret;

    ret = init_gic400();

    if (ret == SUCCESS) {
        wakeup_secondary_processors();
    }
}

static void init_secondary_processor(void)
{
    init_gic400();
}

void init_hw(void)
{
    if (cpu_no() == 0) {
        init_primary_processor();
    } else {
        init_secondary_processor();
    }
}
