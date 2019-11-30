/*
 * test/aarch64/mmu/st1/st1.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/tcr_elx.h"
#include "driver/aarch64/system_register/mair_elx.h"
#include "driver/aarch64/mmu.h"
#include "st1.h"

/* defines */

#define NR_MMU_MEMORY_BLOCKS    1024

/* types */

/* prototypes */

/* variables */

extern char __text_start[];
extern char __text_end[];
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];
extern char __stack_end[];

static char memory_block_region[NR_MMU_MEMORY_BLOCKS][4096] __attribute__ ((aligned(4096)));

static struct aarch64_mmu_block_pool pool;
struct aarch64_mmu test_mmu;

/* functions */

static errno_t map(char *start, char *end, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;
    size_t sz;

    sz = ALIGN((end - start), 4096);
    ret = aarch64_mmu_map(&test_mmu, start, start, sz, attr);

    return ret;
}

static errno_t init_map(void)
{
    errno_t ret;
    struct aarch64_mmu_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.af = 1;
    attr.sh = MMU_ATTR_SH_ISH;
    attr.ap21 = MMU_ATTR_AP_RO;
    attr.attrindx = MMU_ATTR_NC;
    ret = map(__text_start, __text_end, &attr);

    if (ret == SUCCESS) {
        attr.xn = 1;
        ret = map(__rodata_start, __rodata_end, &attr);
    }

    if (ret == SUCCESS) {
        attr.ap21 = MMU_ATTR_AP_RW;
        ret = map(__data_start, __stack_end, &attr);
    }

    /* UART Lite */
    if (ret == SUCCESS) {
        attr.attrindx = MMU_ATTR_DEVICE_nGnRnE;
        ret = map((void*)0xa0001000, (void*)0xa0002000, &attr);
    }

    return ret;
}

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

enum aarch64_mmu_type mmu_type(void)
{
    enum aarch64_mmu_type t;

    switch (aarch64_read_currentel() >> 2) {
    case 1:
        t = AARCH64_MMU_EL1;
        break;
    case 2:
        t = AARCH64_MMU_EL2;
        break;
    case 3:
        t = AARCH64_MMU_EL3;
        break;
    default:
        t = AARCH64_MMU_EL0;
    }

    return t;
}

static errno_t init_mmu(void)
{
    errno_t ret;
    uint64_t pa_range;
    struct aarch64_mmu_configuration config;

    pa_range = aarch64_read_id_aa64mmfr0_el1() & BITS(3, 0);

    memset(&config, 0, sizeof(config));
    config.base.type = mmu_type();
    config.base.granule = AARCH64_MMU_4KB_GRANULE;
    config.base.pool = &pool;
    config.asid = 1;
    config.mair = TEST_ATTRS;
    config.tcr.el23.ps = pa_range;
    config.tcr.el23.sh0 = TCR_SH_ISH;
    config.tcr.el23.orgn0 = TCR_RGN_WBWA;
    config.tcr.el23.irgn0 = TCR_RGN_WBWA;
    config.tcr.el23.t0sz = 16;

    ret = init_pool();
    if (ret == SUCCESS) {
        ret = aarch64_mmu_init(&test_mmu, &config);
    }

    return ret;
}

errno_t test_aarch64_mmu_st1_init_memory_map(void)
{
    errno_t ret;

    ret = init_mmu();
    if (ret == SUCCESS) {
        ret = init_map();
        if (ret == SUCCESS) {
            ret = aarch64_mmu_enable(&test_mmu);
        }
    }

    return ret;
}

