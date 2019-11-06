/*
 * aarch64/mmu/aarch64_mmu_stage2_descriptor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

#define DESC_TABLE  3
#define DESC_BLOCK  1
#define DESC_PAGE   3

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t aarch64_mmu_stage2_table_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr)
{
    uint64_t desc;

    desc = DESC_TABLE;
    desc |= (uint64_t)pa;

    return desc;
}

static uint64_t block_page_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr, uint64_t desc)
{
    desc |= (uint64_t)pa;
    desc |= (uint64_t)attr->xn << 54;
    desc |= (uint64_t)attr->af << 10;
    desc |= (uint64_t)attr->sh << 8;
    desc |= (uint64_t)attr->s2ap << 6;
    desc |= (uint64_t)attr->memattr << 2;

    return desc;
}

uint64_t aarch64_mmu_stage2_block_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr)
{
    uint64_t desc;

    desc = block_page_descriptor(pa, attr, DESC_BLOCK);

    return desc;
}

uint64_t aarch64_mmu_stage2_page_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr)
{
    uint64_t desc;

    desc = block_page_descriptor(pa, attr, DESC_PAGE);

    return desc;
}

