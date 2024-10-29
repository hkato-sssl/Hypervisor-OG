// SPDX-License-Identifier: Apache-2.0
/*
 * init/init_hw.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "driver/aarch64/mmu.h"
#include "driver/aarch64/psci.h"
#include "driver/arm/gic400.h"
#include "driver/arm/smmu500.h"
#include "driver/system/cpu.h"
#include "driver/xilinx/mpsoc.h"
#include "hypervisor/mmu.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

extern struct aarch64_mmu sys_mmu;

struct gic400 sys_gic;
struct smmu500 sys_smmu;

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

static errno_t init_smmu500(void)
{
    errno_t ret;
    struct smmu500_configuration config;
    struct aarch64_mmu_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 1;
    attr.af = 1;
    attr.sh = MMU_ATTR_SH_OSH;
    attr.ap21 = MMU_ATTR_AP_RW;
    attr.attrindx = HYP_MMU_MT_DEVICE_nGnRE;

    memset(&config, 0, sizeof(config));
    config.smmu_base = REG_SMMU500;
    config.mmu = &sys_mmu;
    config.mmu_attr = &attr;
    config.flag.interrupt = 1;

    ret = smmu500_initialize(&sys_smmu, &config);
    if (ret != SUCCESS) {
        printk("smmu500_initialize() -> %d\n", ret);
    }

    if (ret == SUCCESS) {
        struct gic400_interrupt_configuration config;
        memset(&config, 0, sizeof(config));
        config.targets = 1;
        config.priority = 0x01;
        ret = gic400_configure_interrupt(&sys_gic, IRQ_SMMU500, &config);
        if (ret != SUCCESS) {
            printk("gic400_configure_interrupt() -> %d\n", ret);
        }
    }

    return ret;
}

static errno_t init_gic400(void)
{
    errno_t ret;
    struct gic400_configuration config;

    if (cpu_no() == 0) {
        memset(&config, 0, sizeof(config));
        config.base.distributor = (void *)REG_GIC400D;
        config.base.cpuif = (void *)REG_GIC400C;
        config.boolean.priority_drop = true;
        ret = gic400_initialize(&sys_gic, &config);
    } else {
        ret = gic400_initialize(&sys_gic, NULL);
    }

    if (ret != SUCCESS) {
        printk("gic400_initialize() -> %d\n", ret);
    }

    return ret;
}

static void init_primary_processor(void)
{
    errno_t ret;

    ret = init_gic400();

    if (ret == SUCCESS) {
        ret = init_smmu500();
    }

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
