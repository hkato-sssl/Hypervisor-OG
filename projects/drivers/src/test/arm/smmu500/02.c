/*
 * test/arm/smmu500/02.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/mmu.h"
#include "driver/aarch64/stage2.h"
#include "driver/arm/smmu500.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

#define TEST_VMID 1

/* types */

/* prototypes */

/* variables */

extern struct aarch64_mmu sys_mmu;
extern struct aarch64_mmu_block_pool sys_pool;

static uint8_t stage2_table[4096 * 2] __attribute__((aligned(4096 * 2)));
static struct smmu500 smmu;
static struct aarch64_stage2 stage2;

/* functions */

static errno_t map_stage2(void)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 0;
    attr.af = 1;
    attr.sh = STAGE2_SH_OSH;
    attr.s2ap = STAGE2_S2AP_RW;
    attr.memattr = STAGE2_MEMATTR_DEVICE_nGnRnE;
    ret = aarch64_stage2_map(&stage2, (void *)0x40000000, (void *)0x50000000,
                             0x10000, &attr);
    if (ret != SUCCESS) {
        printk("aarch64_stage2_map() -> %d\n", ret);
    }

    return ret;
}

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
    if (ret == SUCCESS) {
        ret = map_stage2();
    }

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
    attr.attrindx = 5; /* Device-nGnRE */

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
    uint8_t cb;
    struct smmu_context_bank_with_stage2_configuration config;

    memset(&config, 0, sizeof(config));
    config.stage2 = &stage2;
    config.interrupt_index = 0;
    config.vmid = 0;
    ret = smmu500_create_context_bank_with_stage2(&smmu, &cb, &config);
    printk("smmu500_create_context_bank_with_stage2() -> %d, cb=%u\n", ret, cb);

    if (ret == SUCCESS) {
        struct smmu_translation_stream_configuration config;
        memset(&config, 0, sizeof(config));
        config.stream.mask = 0x3fff;
        config.stream.id = 0;
        config.cbndx = cb;
        ret = smmu500_create_translation_stream(&smmu, &id, &config);
        printk("smmu500_create_translation_stream() -> %d, id=%u\n", ret, id);
    }

    if (ret == SUCCESS) {
        ret = smmu500_enable(&smmu, id);
        printk("smmu500_enable() -> %d\n", ret);
    }

    smmu500_dump(&smmu);
    smmu500_dump_stream_match_register(&smmu, id);
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

void test_arm_smmu500_02_dump(void)
{
    smmu500_dump(&smmu);
}
