/*
 * init/init_memory_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/tcr.h"
#include "driver/aarch64/mmu.h"
#include "hypervisor/mmu.h"

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

static char memory_block_region[CONFIG_NR_MMU_MEMORY_BLOCKS][4096] __attribute__ ((aligned(4096)));

struct aarch64_mmu sys_mmu;

/* functions */

static errno_t map(char *start, char *end, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;
    size_t sz;

    sz = ALIGN((end - start), 4096);
    ret = aarch64_mmu_map(&sys_mmu, start, start, sz, attr);

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
    attr.attrindx = HYP_MMU_WB;
    ret = map(__text_start, __text_end, &attr);

    if (ret == SUCCESS) {
        attr.xn = 1;
        ret = map(__rodata_start, __rodata_end, &attr);
    }

    if (ret == SUCCESS) {
        attr.ap21 = MMU_ATTR_AP_RW;
        attr.attrindx = HYP_MMU_WBWA;
        ret = map(__data_start, __stack_end, &attr);
    }

    if (ret == SUCCESS) {
	attr.attrindx = HYP_MMU_DEVICE_nGnRnE;
	ret = map((void*)0xa0001000, (void*)0xa0002000, &attr);
    }

    return ret;
}

static errno_t init_mmu(void)
{
    errno_t ret;
    uint64_t pa_range;
    struct aarch64_mmu_configuration config;

    pa_range = aarch64_read_id_aa64mmfr0_el1() & BITS(3, 0);

    memset(&config, 0, sizeof(config));
    config.type = AARCH64_MMU_EL2;
    config.granule = AARCH64_MMU_4KB_GRANULE;
    config.stage1.asid = 1;
    config.stage1.mair = HYP_MMU_ATTRS;
    config.tcr.el23.ps = pa_range;
    config.tcr.el23.sh0 = TCR_SH_ISH;
    config.tcr.el23.orgn0 = TCR_RGN_WBWA;
    config.tcr.el23.irgn0 = TCR_RGN_WBWA;
    config.tcr.el23.t0sz = 16;
    config.pool.block_sz = 4096;
    config.pool.block_region.addr = memory_block_region;
    config.pool.block_region.size = sizeof(memory_block_region);

    ret = aarch64_mmu_init(&sys_mmu, &config);

    return ret;
}

errno_t init_memory_map(void)
{
    errno_t ret;

    ret = init_mmu();
    if (ret == SUCCESS) {
        ret = init_map();
	if (ret == SUCCESS) {
	    ret = aarch64_mmu_enable(&sys_mmu);
	}
    }

    return ret;
}
