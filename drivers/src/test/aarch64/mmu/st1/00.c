/*
 * test/aarch64/mmu/st1/00.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/aarch64/mmu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static char block_pool_region[256][4096] __attribute__ ((aligned(4096)));
static struct aarch64_mmu_trans_table tt;

/* functions */

static errno_t init(void)
{
    errno_t ret;
    struct aarch64_mmu_trans_table_configuration config;

    memset(&config, 0, sizeof(config));
    config.stage = AARCH64_MMU_STAGE1;
    config.granule = AARCH64_MMU_4KB_GRANULE;
    config.stage1.asid = 1;
    config.stage1.mair = 0;
    config.pool.block_sz = 4096;
    config.pool.block_region.addr = block_pool_region;
    config.pool.block_region.size = sizeof(block_pool_region);

    ret = aarch64_mmu_init(&tt, &config);
    printk("aarch64_mmu_init() -> %d\n", ret);

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

    ret = aarch64_mmu_map(&tt, block_pool_region, block_pool_region, 4096, &attr);
    printk("aarch64_mmu_map() -> %d\n", ret);

    return ret;
}

