/*
 * test/aarch64/mmu/st2/st2.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/vtcr_el2.h"
#include "driver/aarch64/stage2.h"
#include "st2.h"

/* defines */

#define NR_MMU_MEMORY_BLOCKS    1024

/* types */

/* prototypes */

/* variables */

static char memory_block_region[NR_MMU_MEMORY_BLOCKS][4096] __attribute__ ((aligned(4096)));

static struct aarch64_mmu_block_pool pool;
struct aarch64_stage2 test_st2_mmu;

/* functions */

static errno_t init_pool(void)
{
    errno_t ret;
    struct aarch64_mmu_block_pool_configuration config;

    memset(&config, 0, sizeof(config));
    config.block_sz = 4096;
    config.block_region.addr = memory_block_region;
    config.block_region.size = sizeof(memory_block_region);
    ret = aarch64_mmu_block_pool_init(&pool, &config);

    return ret;
}

static errno_t init(void)
{
    errno_t ret;
    struct aarch64_stage2_configuration config;

    memset(&config, 0, sizeof(config));
    config.base.type = AARCH64_MMU_STAGE2;
    config.base.granule = AARCH64_MMU_4KB_GRANULE;
    config.base.pool = &pool;
    config.vmid = 1;
    config.pa_range = STAGE2_PA_RANGE_40BITS;
    config.start_level = 0;
    config.sh = STAGE2_SH_ISH;
    config.orgn = STAGE2_RGN_NORMAL_WBWA;
    config.irgn = STAGE2_RGN_NORMAL_WBWA;

    ret = init_pool();
    if (ret == SUCCESS) {
        ret = aarch64_stage2_init(&test_st2_mmu, &config);
    }

    return ret;
}

errno_t test_aarch64_stage2_init_memory_map(void)
{
    errno_t ret;

    ret = init();

    return ret;
}

