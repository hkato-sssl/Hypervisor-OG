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

static uint64_t table_desc(void *addr)
{
    uint64_t desc;

    /* NSTable=0, APTable=0, XNTable=0, PXNTable=0 */
    desc = (uint64_t)addr | 0x03;

    return desc;
}

uint64_t *table_addr(struct aarch64_mmu_trans_table *tt, void *va, uint32_t level)
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
                d = table_desc(new_table);
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

uint64_t *desc_addr(struct aarch64_mmu_trans_table *tt, void *va, uint32_t level)
{
    uint64_t *table;
    uint64_t *desc;

    table = table_addr(tt, va, level);
    if (table != NULL) {
        desc = table + desc_index(va, level);
    } else {
        desc = NULL;
    }

    return desc;
}

static uint64_t descriptor_attrs(struct aarch64_mmu_attr const *attr)
{
    uint64_t d;
    uint64_t upper;
    uint64_t lower;

    /* upper attributes */

    upper = (uint64_t)(attr->xn) << 54;
    upper |= (uint64_t)(attr->pxn) << 53;

    /* lower attributes */

    lower = (uint64_t)(attr->ng) << 11;
    lower |= (uint64_t)(attr->sh) << 8;
    lower |= (uint64_t)(attr->ap21) << 6;
    lower |= (uint64_t)(attr->ns) << 5;
    lower |= (uint64_t)(attr->attrindx) << 2;
    lower |= MMU_DESC_AF;

    /* page descriptor */

    d = upper | lower;

    return d;
}

static uint64_t level3_descriptor(void *pa, struct aarch64_mmu_attr const *attr)
{
    uint64_t d;

    d = descriptor_attrs(attr);

    /* page descriptor */

    d |= (uint64_t)pa | 3;

    return d;
}

static uint64_t block_descriptor(void *pa, struct aarch64_mmu_attr const *attr)
{
    uint64_t d;

    d = descriptor_attrs(attr);

    /* page descriptor */

    d |= (uint64_t)pa | 1;

    return d;
}

static errno_t mmu_map_4KB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;

    desc = desc_addr(tt, va, 3);
    if ((desc != NULL) && ((*desc & BITS(1,0)) == 0)) {
        d = level3_descriptor(pa, attr);
        aarch64_mmu_write_tt(desc, d);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t map_4KB_validate_parameters(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;

    if ((tt != NULL) && (attr != NULL) && IS_ALIGNED((uintptr_t)va, MEM_4KB) && IS_ALIGNED((uintptr_t)pa, MEM_4KB)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_map_4KB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;

    ret = map_4KB_validate_parameters(tt, va, pa, attr);
    if (ret == SUCCESS) {
        ret = mmu_map_4KB(tt, va, pa, attr);
    }

    return ret;
}

static errno_t mmu_map_2MB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;

    desc = desc_addr(tt, va, 2);
    if ((desc != NULL) && ((*desc & BITS(1,0)) == 0)) {
        d = block_descriptor(pa, attr);
        aarch64_mmu_write_tt(desc, d);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t map_2MB_validate_parameters(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;

    if ((tt != NULL) && (attr != NULL) && IS_ALIGNED((uintptr_t)va, MEM_2MB) && IS_ALIGNED((uintptr_t)pa, MEM_2MB)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_map_2MB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;

    ret = map_2MB_validate_parameters(tt, va, pa, attr);
    if (ret == SUCCESS) {
        ret = mmu_map_2MB(tt, va, pa, attr);
    }

    return ret;
}

static errno_t mmu_map_1GB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;
    uint64_t d;
    uint64_t *desc;

    desc = desc_addr(tt, va, 1);
    if ((desc != NULL) && ((*desc & BITS(1,0)) == 0)) {
        d = block_descriptor(pa, attr);
        aarch64_mmu_write_tt(desc, d);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t map_1GB_validate_parameters(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;

    if ((tt != NULL) && (attr != NULL) && IS_ALIGNED((uintptr_t)va, MEM_1GB) && IS_ALIGNED((uintptr_t)pa, MEM_1GB)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_map_1GB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;

    ret = map_1GB_validate_parameters(tt, va, pa, attr);
    if (ret == SUCCESS) {
        ret = mmu_map_1GB(tt, va, pa, attr);
    }

    return ret;
}
