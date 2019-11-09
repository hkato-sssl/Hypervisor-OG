/*
 * aarch64/mmu/aarch64_mmu_map_4KB.c - mapping function with 4KB granule
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint64_t table_descriptor(struct aarch64_mmu *mmu, void *pa, union mmu_attr const *attr)
{
    uint64_t d;

    if (mmu->stage == AARCH64_MMU_STAGE1) {
        d = aarch64_mmu_table_descriptor(pa, attr->stage1);
    } else {
        d = aarch64_mmu_stage2_table_descriptor(pa, attr->stage2);
    }

    return d;
}

static uint64_t block_descriptor(struct aarch64_mmu *mmu, void *pa, union mmu_attr const *attr)
{
    uint64_t d;

    if (mmu->stage == AARCH64_MMU_STAGE1) {
        d = aarch64_mmu_block_descriptor(pa, attr->stage1);
    } else {
        d = aarch64_mmu_stage2_block_descriptor(pa, attr->stage2);
    }

    return d;
}

static uint64_t page_descriptor(struct aarch64_mmu *mmu, void *pa, union mmu_attr const *attr)
{
    uint64_t d;

    if (mmu->stage == AARCH64_MMU_STAGE1) {
        d = aarch64_mmu_page_descriptor(pa, attr->stage1);
    } else {
        d = aarch64_mmu_stage2_page_descriptor(pa, attr->stage2);
    }

    return d;
}

static uint64_t block_page_descriptor(struct aarch64_mmu *mmu, void *pa, union mmu_attr const *attr, uint32_t level)
{
    uint64_t d;

    if (level == 3) {
        d = page_descriptor(mmu, pa, attr);
    } else {
        d = block_descriptor(mmu, pa, attr);
    }

    return d;
}

static void write_descriptor(uint64_t *desc, uint64_t d)
{
    *desc = d;
    aarch64_dcache_clean(desc);
}

static uint64_t *next_table_addr(uint64_t desc)
{
    uint64_t *addr;

    addr = (uint64_t *)(desc & BITS(47, 12));

    return addr;
}

static uint32_t desc_index(void *va, uint32_t level)
{
    uint32_t index;
    uint32_t lsb;

    lsb = (3 - level) * 9 + 12;
    index = (uint32_t)BF_EXTRACT((uintptr_t)va, (lsb + 8), lsb);

    return index;
}

uint64_t *table_addr(struct aarch64_mmu *mmu, void *va, uint32_t level, union mmu_attr const *attr)
{
    uint32_t i;
    uint64_t d;
    uint64_t *table;
    uint64_t *next;
    uint64_t *new_table;

    table = mmu->addr;
    for (i = 1; i <= level; ++i) {
        next = table + desc_index(va, (i - 1));
        d = *next;
        if ((d & BIT(0)) == 0) {
            /* invalid descriptor */
            new_table = aarch64_mmu_block_calloc(&(mmu->pool), MEM_4KB);
            if (new_table != NULL) {
                d = table_descriptor(mmu, new_table, attr);
                write_descriptor(next, d);
                table = new_table;
            } else {
                table = NULL;
                break;
            }
        } else if ((d & BITS(1, 0)) == 0x01) {
            /* block descriptor */
            table = NULL; /* no support */
            break;
        } else {
            /* d[1:0] == 0b11, table descriptor */
            table = next_table_addr(d);
        }
    }

    return table;
}

uint64_t *desc_addr(struct aarch64_mmu *mmu, void *va, union mmu_attr const *attr, uint32_t level)
{
    uint64_t *table;
    uint64_t *desc;

    table = table_addr(mmu, va, level, attr);
    if (table != NULL) {
        desc = table + desc_index(va, level);
    } else {
        desc = NULL;
    }

    return desc;
}

static bool is_unmapped_contiguous_region(uint64_t const *desc)
{
    bool ret;
    int i;

    ret = true;
    for (i = 0; i < 16; ++i) {
        if ((desc[i] & BIT(0)) != 0) {
            ret = false;
            break;
        }
    }

    return ret;
}

static bool is_valid_region_parameter(void *va, void *pa, size_t size)
{
    bool valid;

    if (IS_ALIGNED((uintptr_t)va, size) && IS_ALIGNED((uintptr_t)pa, size)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

static errno_t mmu_map_contiguous_region(struct aarch64_mmu *mmu, void *va, void *pa, union mmu_attr const *attr, uint32_t level, size_t size)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;
    int i;

    if (is_valid_region_parameter(va, pa, size)) {
        desc = desc_addr(mmu, va, attr, level);
        if (is_unmapped_contiguous_region(desc)) {
            d = block_page_descriptor(mmu, pa, attr, level);
            d |= MMU_DESC_CONTIGUOUS;
            for (i = 0; i < 16; ++i) {
                desc[i] = d;
            }
            ret = aarch64_dcache_clean_range(desc, (sizeof(*desc) * 16));
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_map_single_region(struct aarch64_mmu *mmu, void *va, void *pa, union mmu_attr const *attr, uint32_t level, size_t size)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;

    if (is_valid_region_parameter(va, pa, size)) {
        desc = desc_addr(mmu, va, attr, level);
        if ((desc != NULL) && ((*desc & BITS(1,0)) == 0)) {
            d = block_page_descriptor(mmu, pa, attr, level);
            write_descriptor(desc, d);
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_map(struct aarch64_mmu *mmu, void **va, void **pa, size_t *sz, union mmu_attr const *attr)
{
    errno_t ret;

    if ((*sz >= MEM_16GB) && ((ret = mmu_map_contiguous_region(mmu, *va, *pa, attr, 1, MEM_16GB)) == SUCCESS)) {
        *(char **)va += MEM_16GB;
        *(char **)pa += MEM_16GB;
        *sz -= MEM_16GB;
    } else if ((*sz >= MEM_1GB) && ((ret = mmu_map_single_region(mmu, *va, *pa, attr, 1, MEM_1GB)) == SUCCESS)) {
        *(char **)va += MEM_1GB;
        *(char **)pa += MEM_1GB;
        *sz -= MEM_1GB;
    } else if ((*sz >= MEM_32MB) && ((ret = mmu_map_contiguous_region(mmu, *va, *pa, attr, 2, MEM_32MB)) == SUCCESS)) {
        *(char **)va += MEM_32MB;
        *(char **)pa += MEM_32MB;
        *sz -= MEM_32MB;
    } else if ((*sz >= MEM_2MB) && ((ret = mmu_map_single_region(mmu, *va, *pa, attr, 2, MEM_2MB)) == SUCCESS)) {
        *(char **)va += MEM_2MB;
        *(char **)pa += MEM_2MB;
        *sz -= MEM_2MB;
    } else if ((*sz >= MEM_64KB) && ((ret = mmu_map_contiguous_region(mmu, *va, *pa, attr, 3, MEM_64KB)) == SUCCESS)) {
        *(char **)va += MEM_64KB;
        *(char **)pa += MEM_64KB;
        *sz -= MEM_64KB;
    } else if ((*sz >= MEM_4KB) && ((ret = mmu_map_single_region(mmu, *va, *pa, attr, 3, MEM_4KB)) == SUCCESS)) {
        *(char **)va += MEM_4KB;
        *(char **)pa += MEM_4KB;
        *sz -= MEM_4KB;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t validate_mmu_structures(struct aarch64_mmu *mmu, union mmu_attr const *attr)
{
    errno_t ret;

    if ((mmu != NULL) && (attr != NULL)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_map_4KB_granule(struct aarch64_mmu *mmu, void *va, void *pa, size_t sz, union mmu_attr const *attr)
{
    errno_t ret;

    ret = validate_mmu_structures(mmu, attr);
    if (ret == SUCCESS) {
        do {
            ret = mmu_map(mmu, &va, &pa, &sz, attr);
        } while ((ret == SUCCESS) && (sz != 0));
    } 

    return ret;
}

