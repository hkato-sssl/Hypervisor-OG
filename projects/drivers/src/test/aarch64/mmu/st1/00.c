// SPDX-License-Identifier: Apache-2.0
/*
 * test/aarch64/mmu/st1/00.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/mmu.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/mair_elx.h"
#include "driver/aarch64/system_register/tcr_elx.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* テスト項目：MMU 有効化
 *
 * プログラムが動作可能な必要最低限のMappingを行い
 * MMU を有効化した状態で動作可能である事を確認。
 */

/* test parameters */

#define RAM_START 0x00000000
#define RAM_SIZE  0x80000000
#define DEV_START 0xa0001000
#define DEV_SIZE  4096

/* defines */

#define ATTRS                                                                  \
    (MAIR_ATTR(0, MAIR_ATTR_NORMAL_NC) | MAIR_ATTR(1, MAIR_ATTR_NORMAL_WB)     \
     | MAIR_ATTR(2, MAIR_ATTR_NORMAL_WBWA) | MAIR_ATTR(3, MAIR_ATTR_NORMAL_NC) \
     | MAIR_ATTR(4, MAIR_ATTR_DEVICE_nGnRnE)                                   \
     | MAIR_ATTR(5, MAIR_ATTR_DEVICE_nGnRE)                                    \
     | MAIR_ATTR(6, MAIR_ATTR_DEVICE_nGRE)                                     \
     | MAIR_ATTR(7, MAIR_ATTR_DEVICE_GRE))

#define MMU_ATTR_NC            0
#define MMU_ATTR_WB            1
#define MMU_ATTR_WBWA          2
#define MMU_ATTR_DEVICE_nGnRnE 4
#define MMU_ATTR_DEVICE_nGnRE  5
#define MMU_ATTR_DEVICE_nGRE   6
#define MMU_ATTR_DEVICE_GRE    7

/* types */

/* prototypes */

/* variables */

static char block_pool_region[256][4096] __attribute__((aligned(4096)));
static char tmp[65536] __attribute__((aligned(65536)));
static struct aarch64_mmu_block_pool pool;
static struct aarch64_mmu mmu;

/* functions */

static errno_t init_pool(void)
{
    errno_t ret;
    struct aarch64_mmu_block_pool_configuration config;

    memset(&config, 0, sizeof(config));
    config.block_sz = 4096;
    config.block_region.addr = block_pool_region;
    config.block_region.size = sizeof(block_pool_region);
    ret = aarch64_mmu_block_pool_initialize(&pool, &config);
    printk("aarch64_mmu_block_pool_initialize() -> %d\n", ret);

    return ret;
}

static errno_t init(void)
{
    errno_t ret;
    uint64_t pa_range;
    struct aarch64_mmu_configuration config;

    ret = init_pool();
    if (ret != SUCCESS) {
        return ret;
    }

    pa_range = aarch64_read_id_aa64mmfr0_el1() & BITS(3, 0);

    memset(&config, 0, sizeof(config));
    config.base.type = AARCH64_MMU_EL2;
    config.base.granule = AARCH64_MMU_4KB_GRANULE;
    config.base.pool = &pool;
    config.asid = 0;
    config.mair = ATTRS;
    config.tcr.el23.ps = pa_range;
    config.tcr.el23.sh0 = TCR_SH_ISH;
    config.tcr.el23.orgn0 = TCR_RGN_WBWA;
    config.tcr.el23.irgn0 = TCR_RGN_WBWA;
    config.tcr.el23.t0sz = 16;

    ret = aarch64_mmu_initialize(&mmu, &config);
    printk("aarch64_mmu_initialize() -> %d\n", ret);

    return ret;
}

errno_t test_aarch64_mmu_st1_00(void)
{
    errno_t ret;
    struct aarch64_mmu_attr attr;

    printk("<%s>\n", __func__);

    ret = init();
    if (ret != SUCCESS) {
        return ret;
    }

    memset(&attr, 0, sizeof(attr));

    ret = aarch64_mmu_map(&mmu, block_pool_region, block_pool_region, 4096,
                          &attr);
    printk("aarch64_mmu_map() -> %d\n", ret);

    ret = aarch64_mmu_map(&mmu, tmp, tmp, sizeof(tmp), &attr);
    printk("aarch64_mmu_map() -> %d\n", ret);

    aarch64_mmu_dump_descriptor(&mmu);

    return ret;
}
