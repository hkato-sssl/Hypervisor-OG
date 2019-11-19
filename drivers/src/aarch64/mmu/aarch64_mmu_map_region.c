/*
 * aarch64/mmu/aarch64_mmu_map_region.c
 *
 * (e) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu_base.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

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

uint64_t *new_table(struct aarch64_mmu_base *mmu, void const *attr, uint64_t *previous)
{
    void *table;
    uint64_t d;

    table = aarch64_mmu_block_calloc(mmu->pool, MEM_4KB);
    if (table != NULL) {
        d = (mmu->ops->table_descriptor)(table, attr);
        d |= (uint64_t)table;
        aarch64_mmu_write_descriptor(previous, d);
    }

    return table;
}

uint64_t *table_addr(struct aarch64_mmu_base *mmu, void *va, void const *attr, uint32_t level)
{
    uint32_t i;
    uint64_t d;
    uint64_t *table;
    uint64_t *next;

    table = mmu->addr;
    for (i = 1; ((table != NULL) && (i <= level)); ++i) {
        next = table + desc_index(va, (i - 1));
        d = *next;
        if ((d & BIT(0)) == 0) {
            /* invalid descriptor */
            table = new_table(mmu, attr, next);
        } else if ((d & BITS(1, 0)) == 0x01) {
            /* block descriptor */
            table = NULL; /* no support */
        } else {
            /* d[1:0] == 0b11, table descriptor */
            table = next_table_addr(d);
        }
    }

    return table;
}

uint64_t *desc_addr(struct aarch64_mmu_base *mmu, void *va, void const *attr, uint32_t level)
{
    uint64_t *table;
    uint64_t *desc;

    table = table_addr(mmu, va, attr, level);
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

static bool is_valid_parameter(void *va, void *pa, size_t sz)
{
    bool valid;

    if (IS_ALIGNED(va, sz) && IS_ALIGNED(pa, sz)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t aarch64_mmu_map_contiguous_region(struct aarch64_mmu_base *mmu, void *va, void *pa, size_t sz, void const *attr, uint32_t level)
{
    errno_t ret;
    uint64_t *p;
    uint64_t desc;
    int i;

    if (is_valid_parameter(va, pa, sz)) {
        p = desc_addr(mmu, va, attr, level);
        if (is_unmapped_contiguous_region(p)) {
            if (level == 3) {
                desc = (mmu->ops->page_descriptor)(pa, attr);
            } else {
                desc = (mmu->ops->block_descriptor)(pa, attr);
            }
            desc |= MMU_DESC_CONTIGUOUS;
            for (i = 0; i < 16; ++i) {
                p[i] = desc;
            }
            ret = aarch64_dcache_clean_range(p, (sizeof(*p) * 16));
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_map_single_region(struct aarch64_mmu_base *mmu, void *va, void *pa, size_t sz, void const *attr, uint32_t level)
{
    errno_t ret;
    uint64_t *p;
    uint64_t desc;

    if (is_valid_parameter(va, pa, sz)) {
        p = desc_addr(mmu, va, attr, level);
        if ((p != NULL) && ((*p & BITS(1,0)) == 0)) {
            if (level == 3) {
                desc = (mmu->ops->page_descriptor)(pa, attr);
            } else {
                desc = (mmu->ops->block_descriptor)(pa, attr);
            }
            aarch64_mmu_write_descriptor(p, desc);
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}
