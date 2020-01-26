/*
 * init/init_hw.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/aarch64/psci.h"
#include "driver/xilinx/mpsoc.h"
#include "driver/system/cpu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

struct gic400 sys_gic;

/* functions */

static void wakeup_secondary_processors(void)
{
    int i;
    errno_t ret;
    void _start(void);

    for (i = 1; i < NR_CPUS; ++i) {
        ret = aarch64_psci_cpu_on(i, _start, 0);
        if (ret != SUCCESS) {
            printk("aarch64_psci_cpu_on(%d) -> %d\n", i, ret);
        }
    }
}

void init_hw(void)
{
    errno_t ret;
    struct gic400_configuration config;

    if (cpu_no() == 0) {
        memset(&config, 0, sizeof(config));
        config.base.distributor = (void *)GIC400D_BASE;
        config.base.cpuif = (void *)GIC400C_BASE;
        ret = gic400_init(&sys_gic, &config);
    } else {
        ret = gic400_init(&sys_gic, NULL);
    }

    if (ret == SUCCESS) {
        if (cpu_no() == 0) {
            wakeup_secondary_processors();
        }
    } else {
        printk("gic400_init() -> %d", ret);
    }
}
