/*
 * test/stage2/01.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/aarch64/mmu.h"

/* defines */

/* types */

/* prototypes */

void test_stage2_01_entry(void);

/* variables */

static char mem_block_region[32][4096] __attribute__((aligned(4096)));
static struct aarch64_mmu_trans_table st2;

/* functions */

void init(void)
{
    errno_t ret;
    struct aarch64_mmu_trans_table_configuration config;

    config.stage = MMU_STAGE1;
    config.stage2.vmid = 0;
    //config.tcr.
    config.pool.block_sz = 4096;
    config.pool.block_region.addr = mem_block_region;
    config.pool.block_region.size = sizeof(mem_block_region);

    ret = aarch64_mmu_init(&st2, &config);
    printk("aarch64_mmu_init() -> %d\n", ret);
}

void test_stage2_01(void)
{
    struct aarch64_mmu_attr attr;

    printk("<%s>\n", __func__);
    init();

    attr.stage = MMU_STAGE2;
    attr.stage2.xn = 0;
    attr.stage2.sh = MMU_ATTR_ISH;
    attr.stage2.s2ap = MMU_ATTR_S2AP = RW;
    attr.stage2.memattr = 0;


}

