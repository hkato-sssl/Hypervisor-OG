// SPDX-License-Identifier: Apache-2.0
/*
 * test/arm/smmu500/01.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/mmu.h"
#include "driver/arm/smmu500.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

extern struct aarch64_mmu sys_mmu;

static struct smmu500 smmu;

/* functions */

static void init(struct smmu500 *smmu)
{

    errno_t ret;
    struct smmu500_configuration config;
    struct aarch64_mmu_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 1;
    attr.af = 1;
    attr.sh = MMU_ATTR_SH_OSH;
    attr.ap21 = MMU_ATTR_AP_RW;
    attr.attrindx = 5; /* Device-nGnRE */

    memset(&config, 0, sizeof(config));
    config.smmu_base = 0xfd800000;
    config.mmu = &sys_mmu;
    config.mmu_attr = &attr;

    ret = smmu500_initialize(smmu, &config);
    printk("smmu500_initialize() -> %d\n", ret);

    smmu500_dump(smmu);
}

void test_arm_smmu500_01(void)
{
    printk("<%s>\n", __func__);

    init(&smmu);
}
