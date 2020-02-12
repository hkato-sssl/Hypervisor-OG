/*
 * hypervisor/test/stage2.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

struct aarch64_stage2 hyp_test_stage2;
static struct aarch64_mmu_block_pool pool;
static char level1_table[4096 * 2] __attribute__ ((aligned(4096 * 2)));
static char memory_block_region[4096][32] __attribute__ ((aligned(4096)));

/* functions */

static errno_t init_pool(void)
{
    errno_t ret;
    struct aarch64_mmu_block_pool_configuration config;

    memset(&config, 0, sizeof(config));
    config.block_sz = 4096;
    config.block_region.addr = memory_block_region;
    config.block_region.size = sizeof(memory_block_region);
    ret = aarch64_mmu_block_pool_initialize(&pool, &config);

    return ret;
}

errno_t hyp_test_stage2_init(void)
{
    errno_t ret;
    struct aarch64_stage2_configuration config;

    memset(&config, 0, sizeof(config));
    config.base.type = AARCH64_MMU_STAGE2;
    config.base.granule = AARCH64_MMU_4KB_GRANULE;
    config.base.pool = &pool;
    config.vmid = 1;
    config.pa_range = STAGE2_PA_RANGE_40BITS;
    config.start_level = 1;
    config.sh = STAGE2_SH_ISH;
    config.orgn = STAGE2_RGN_NORMAL_WBWA;
    config.irgn = STAGE2_RGN_NORMAL_WBWA;
    config.first_table = level1_table;

    ret = init_pool();
    if (ret == SUCCESS) {
        ret = aarch64_stage2_init(&hyp_test_stage2, &config);
    }

    return ret;
}

