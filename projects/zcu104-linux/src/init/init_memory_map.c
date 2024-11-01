// SPDX-License-Identifier: Apache-2.0
/*
 * init/init_memory_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"

#include "driver/aarch64/mmu.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/tcr_elx.h"
#include "driver/arm/device/gic400.h"
#include "driver/system/cpu.h"
#include "driver/xilinx/device/mpsoc.h"
#include "hypervisor/mmu.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

extern char __text_start[];
extern char __text_end[];
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];
extern char __stack_end[];

static char memory_block_region[CONFIG_NR_MMU_MEMORY_BLOCKS][4096]
    __attribute__((aligned(4096)));

struct aarch64_mmu_block_pool sys_pool;
struct aarch64_mmu sys_mmu;

/* functions */

static errno_t map(char *start, char *end, const struct aarch64_mmu_attr *attr)
{
    errno_t ret;
    size_t sz;

    sz = ALIGN((end - start), 4096);
    ret = aarch64_mmu_map(&sys_mmu, start, start, sz, attr);
    if (ret != SUCCESS) {
        printk("map(start=%p, end=%p) -> %d\n", start, end, ret);
    }

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
    attr.attrindx = HYP_MMU_MT_NORMAL_WB;
    ret = map(__text_start, __text_end, &attr);

    if (ret == SUCCESS) {
        attr.xn = 1;
        ret = map(__rodata_start, __rodata_end, &attr);
    }

    if (ret == SUCCESS) {
        attr.ap21 = MMU_ATTR_AP_RW;
        attr.attrindx = HYP_MMU_MT_NORMAL_WBWA;
        ret = map(__data_start, __stack_end, &attr);
    }

    if (ret == SUCCESS) {
        attr.sh = MMU_ATTR_SH_OSH;
        attr.attrindx = HYP_MMU_MT_DEVICE_nGnRE;
        ret = map((void *)CONFIG_GICD_BASE, (void *)(CONFIG_GICD_BASE + 4096),
                  &attr);
    }

    if (ret == SUCCESS) {
        attr.sh = MMU_ATTR_SH_NSH;
        attr.attrindx = HYP_MMU_MT_DEVICE_nGnRE;
        ret = map((void *)CONFIG_GICC_BASE, (void *)(CONFIG_GICC_BASE + 4096),
                  &attr);
    }

    if (ret == SUCCESS) {
        ret = map((void *)(CONFIG_GICC_BASE + GICC_DIR),
                  (void *)(CONFIG_GICC_BASE + GICC_DIR + 4096), &attr);
    }

    if (ret == SUCCESS) {
        attr.sh = MMU_ATTR_SH_NSH;
        attr.attrindx = HYP_MMU_MT_DEVICE_nGnRE;
        ret = map((void *)CONFIG_GICH_BASE, (void *)(CONFIG_GICH_BASE + 4096),
                  &attr);
    }

    if (ret == SUCCESS) {
        attr.sh = MMU_ATTR_SH_NSH;
        attr.attrindx = HYP_MMU_MT_DEVICE_nGnRE;
        ret = map((void *)CONFIG_GICV_BASE,
                  (void *)(CONFIG_GICV_BASE + 4096 * 2), &attr);
    }

    if (ret == SUCCESS) {
        attr.sh = MMU_ATTR_SH_OSH;
        attr.attrindx = HYP_MMU_MT_DEVICE_nGnRE;
        ret = map((void *)REG_SMMU500, (void *)(REG_SMMU500 + 4096), &attr);
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
    ret = aarch64_mmu_block_pool_initialize(&sys_pool, &config);

    return ret;
}

static errno_t init_mmu(void)
{
    errno_t ret;
    uint64_t pa_range;
    struct aarch64_mmu_configuration config;

    pa_range = aarch64_read_id_aa64mmfr0_el1() & BITS(3, 0);

    memset(&config, 0, sizeof(config));
    config.base.type = AARCH64_MMU_EL2;
    config.base.granule = AARCH64_MMU_4KB_GRANULE;
    config.base.pool = &sys_pool;
    config.asid = 1;
    config.mair = HYP_MMU_ATTRS;
    config.tcr.el23.ps = pa_range;
    config.tcr.el23.sh0 = TCR_SH_ISH;
    config.tcr.el23.orgn0 = TCR_RGN_WBWA;
    config.tcr.el23.irgn0 = TCR_RGN_WBWA;
    config.tcr.el23.t0sz = 16;

    ret = init_pool();
    if (ret == SUCCESS) {
        ret = aarch64_mmu_initialize(&sys_mmu, &config);
    }

    return ret;
}

errno_t init_memory_map(void)
{
    errno_t ret;
    uint8_t no;

    no = cpu_no();
    if (no == 0) {
        ret = init_mmu();
        if (ret == SUCCESS) {
            ret = init_map();
            if (ret == SUCCESS) {
                ret = aarch64_mmu_enable(&sys_mmu);
            }
        }
    } else {
        ret = aarch64_mmu_enable(&sys_mmu);
    }

    return ret;
}
