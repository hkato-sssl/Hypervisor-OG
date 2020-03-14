/*
 * test/arm/smmu500/02.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/aarch64/mmu.h"
#include "driver/aarch64/stage2.h"
#include "driver/arm/smmu500.h"

/* defines */

#define TEST_VMID       1

/* types */

/* prototypes */

/* variables */

extern struct aarch64_mmu sys_mmu;
extern struct aarch64_mmu_block_pool sys_pool;

static uint8_t stage2_table[4096 * 2] __attribute__ ((aligned(4096 * 2)));
static struct smmu500 smmu;
static struct aarch64_stage2 stage2;

/* functions */

static errno_t init_stage2(void)
{
    errno_t ret;
    struct aarch64_stage2_configuration config;

    memset(&config, 0, sizeof(config));
    config.base.type = AARCH64_MMU_STAGE2;
    config.base.granule = AARCH64_MMU_4KB_GRANULE;
    config.base.pool = &sys_pool;
    config.vmid = TEST_VMID;
    config.pa_range = STAGE2_PA_RANGE_40BITS;
    config.start_level = 1;
    config.sh = STAGE2_SH_ISH;
    config.orgn = STAGE2_RGN_NORMAL_WBWA;
    config.irgn = STAGE2_RGN_NORMAL_WBWA;
    config.first_table = stage2_table;
    ret = aarch64_stage2_initialize(&stage2, &config);

    return ret;
}


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
    attr.attrindx = 5;          /* Device-nGnRE */

    memset(&config, 0, sizeof(config));
    config.smmu_base = 0xfd800000;
    config.mmu = &sys_mmu;
    config.mmu_attr = &attr;

    ret = smmu500_initialize(smmu, &config);
    printk("smmu500_initialize() -> %d\n", ret);
}

static void test_02(void)
{
    errno_t ret;
    uint8_t id;
    struct smmu500_attach_stage2_configuration config;

    memset(&config, 0, sizeof(config));

    config.stream.mask = 0xffff;
    config.stream.id = 0x1234;
    config.stage2 = &stage2;
    config.translation.flag.write_allocate = 1;
    config.translation.flag.read_allocate = 1;
    config.translation.flag.extended_id = 1;

    ret = smmu500_attach_stage2(&id, &smmu, &config);
    printk("smmu500_attach_stage2() -> %d, id=%u\n", ret, id);

    smmu500_dump(&smmu);
}

void test_arm_smmu500_02(void)
{
    errno_t ret;

    printk("<%s>\n", __func__);

    init(&smmu);
    ret = init_stage2();
    printk("init_stage2() -> %d\n", ret);

    test_02();
}

