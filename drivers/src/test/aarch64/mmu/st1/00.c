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
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/mair.h"
#include "driver/aarch64/system_register/tcr.h"

/* defines */

#define ATTRS       (MAIR_ATTR(0, MAIR_ATTR_NORMAL_NC) |\
                     MAIR_ATTR(1, MAIR_ATTR_NORMAL_WB) |\
                     MAIR_ATTR(2, MAIR_ATTR_NORMAL_WBWA) |\
                     MAIR_ATTR(3, MAIR_ATTR_NORMAL_NC) |\
                     MAIR_ATTR(4, MAIR_ATTR_DEVICE_nGnRnE) | \
                     MAIR_ATTR(5, MAIR_ATTR_DEVICE_nGnRE) | \
                     MAIR_ATTR(6, MAIR_ATTR_DEVICE_nGRE) | \
                     MAIR_ATTR(7, MAIR_ATTR_DEVICE_GRE))

/* types */

/* prototypes */

/* variables */

static char block_pool_region[256][4096] __attribute__ ((aligned(4096)));
static struct aarch64_mmu mmu;
static char tmp[65536] __attribute__ ((aligned(65536)));

/* functions */

static errno_t init(void)
{
    errno_t ret;
    uint64_t pa_range;
    struct aarch64_mmu_configuration config;

    pa_range = aarch64_read_id_aa64mmfr0_el1() & BITS(3, 0);

    memset(&config, 0, sizeof(config));
    config.type = AARCH64_MMU_EL1;
    config.granule = AARCH64_MMU_4KB_GRANULE;
    config.stage1.asid = 1;
    config.stage1.mair = ATTRS;
    config.tcr.el1.as = 1;
    config.tcr.el1.ips = pa_range;
    config.tcr.el1.sh1 = TCR_SH_ISH;
    config.tcr.el1.orgn1 = TCR_RGN_WB;
    config.tcr.el1.irgn1 = TCR_RGN_WB;
    config.tcr.el1.t1sz = 16;

    config.pool.block_sz = 4096;
    config.pool.block_region.addr = block_pool_region;
    config.pool.block_region.size = sizeof(block_pool_region);

    ret = aarch64_mmu_init(&mmu, &config);
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

    ret = aarch64_mmu_map(&mmu, block_pool_region, block_pool_region, 4096, &attr);
    printk("aarch64_mmu_map() -> %d\n", ret);

    ret = aarch64_mmu_map(&mmu, tmp, tmp, sizeof(tmp), &attr);
    printk("aarch64_mmu_map() -> %d\n", ret);

    aarch64_mmu_dump_descriptor(&mmu);

    return ret;
}

