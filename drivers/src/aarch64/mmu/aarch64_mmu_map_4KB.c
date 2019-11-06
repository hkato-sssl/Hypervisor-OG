/*
 * aarch64/mmu/aarch64_mmu_map_4KB.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint64_t table_descriptor(struct aarch64_mmu_trans_table *tt, void *pa, union mmu_attr const *attr)
{
    uint64_t d;

    if (tt->stage == AARCH64_MMU_STAGE1) {
        d = aarch64_mmu_table_descriptor(pa, attr->stage1);
    } else {
        d = aarch64_mmu_stage2_table_descriptor(pa, attr->stage2);
    }

    return d;
}

static uint64_t block_descriptor(struct aarch64_mmu_trans_table *tt, void *pa, union mmu_attr const *attr)
{
    uint64_t d;

    if (tt->stage == AARCH64_MMU_STAGE1) {
        d = aarch64_mmu_block_descriptor(pa, attr->stage1);
    } else {
        d = aarch64_mmu_stage2_block_descriptor(pa, attr->stage2);
    }

    return d;
}

static uint64_t page_descriptor(struct aarch64_mmu_trans_table *tt, void *pa, union mmu_attr const *attr)
{
    uint64_t d;

    if (tt->stage == AARCH64_MMU_STAGE1) {
        d = aarch64_mmu_page_descriptor(pa, attr->stage1);
    } else {
        d = aarch64_mmu_stage2_page_descriptor(pa, attr->stage2);
    }

    return d;
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

uint64_t *table_addr(struct aarch64_mmu_trans_table *tt, void *va, uint32_t level, union mmu_attr const *attr)
{
    uint32_t i;
    uint64_t d;
    uint64_t *table;
    uint64_t *next;
    uint64_t *new_table;

    table = tt->addr;
    for (i = 1; i <= level; ++i) {
        next = table + desc_index(va, (i - 1));
        d = *next;
        if ((d & BIT(0)) == 0) {
            /* invalid descriptor */
            new_table = aarch64_mmu_block_calloc(&(tt->pool), MEM_4KB);
            if (new_table != NULL) {
                d = table_descriptor(tt, new_table, attr);
                aarch64_mmu_write_tt(next, d);
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
            /* d[1;0] == 0b11, table descriptor */
            table = next_table_addr(d);
        }
    }

    return table;
}

uint64_t *desc_addr(struct aarch64_mmu_trans_table *tt, void *va, uint32_t level, union mmu_attr const *attr)
{
    uint64_t *table;
    uint64_t *desc;

    table = table_addr(tt, va, level, attr);
    if (table != NULL) {
        desc = table + desc_index(va, level);
    } else {
        desc = NULL;
    }

    return desc;
}

static errno_t map_4KB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;

    desc = desc_addr(tt, va, 3, attr);
    if ((desc != NULL) && ((*desc & BITS(1,0)) == 0)) {
        d = page_descriptor(tt, pa, attr);
        aarch64_mmu_write_tt(desc, d);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t map_4KB_validate_parameters(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;

    if ((tt != NULL) && (attr != NULL) && (tt->stage == AARCH64_MMU_STAGE1) && IS_ALIGNED((uintptr_t)va, MEM_4KB) && IS_ALIGNED((uintptr_t)pa, MEM_4KB)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_map_4KB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;

    ret = map_4KB_validate_parameters(tt, va, pa, attr);
    if (ret == SUCCESS) {
        ret = map_4KB(tt, va, pa, attr);
    }

    return ret;
}

static errno_t map_2MB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;

    desc = desc_addr(tt, va, 2, attr);
    if ((desc != NULL) && ((*desc & BITS(1,0)) == 0)) {
        d = block_descriptor(tt, pa, attr);
        aarch64_mmu_write_tt(desc, d);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t map_2MB_validate_parameters(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;

    if ((tt != NULL) && (attr != NULL) && (tt->stage == AARCH64_MMU_STAGE1) && IS_ALIGNED((uintptr_t)va, MEM_2MB) && IS_ALIGNED((uintptr_t)pa, MEM_2MB)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_map_2MB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;

    ret = map_2MB_validate_parameters(tt, va, pa, attr);
    if (ret == SUCCESS) {
        ret = map_2MB(tt, va, pa, attr);
    }

    return ret;
}

static errno_t map_1GB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;

    desc = desc_addr(tt, va, 1, attr);
    if ((desc != NULL) && ((*desc & BITS(1,0)) == 0)) {
        d = block_descriptor(tt, pa, attr);
        aarch64_mmu_write_tt(desc, d);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t map_1GB_validate_parameters(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;

    if ((tt != NULL) && (attr != NULL) && (tt->stage == AARCH64_MMU_STAGE1) && IS_ALIGNED((uintptr_t)va, MEM_1GB) && IS_ALIGNED((uintptr_t)pa, MEM_1GB)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_map_1GB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, union mmu_attr const *attr)
{
    errno_t ret;

    ret = map_1GB_validate_parameters(tt, va, pa, attr);
    if (ret == SUCCESS) {
        ret = map_1GB(tt, va, pa, attr);
    }

    return ret;
}

static errno_t mmu_map(struct aarch64_mmu_trans_table *tt, void **va, void **pa, size_t *sz, union mmu_attr const *attr)
{
    errno_t ret;

    if ((*sz >= MEM_1GB) && ((ret = mmu_map_1GB(tt, *va, *pa, attr)) == SUCCESS)) {
        *(char **)va += MEM_1GB;
        *(char **)pa += MEM_1GB;
        *sz -= MEM_1GB;
    } else if ((*sz >= MEM_2MB) && ((ret = mmu_map_2MB(tt, *va, *pa, attr)) == SUCCESS)) {
        *(char **)va += MEM_2MB;
        *(char **)pa += MEM_2MB;
        *sz -= MEM_2MB;
    } else if ((*sz >= MEM_4KB) && ((ret = mmu_map_4KB(tt, *va, *pa, attr)) == SUCCESS)) {
        *(char **)va += MEM_4KB;
        *(char **)pa += MEM_4KB;
        *sz -= MEM_4KB;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_map_4KB_granule(struct aarch64_mmu_trans_table *tt, void *va, void *pa, size_t sz, union mmu_attr const *attr)
{
    errno_t ret;

    do {
        ret = mmu_map(tt, &va, &pa, &sz, attr);
    } while ((ret == SUCCESS) && (sz != 0));

    return ret;
}
