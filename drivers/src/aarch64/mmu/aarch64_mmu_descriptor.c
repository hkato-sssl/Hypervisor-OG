/*
 * aarch64/mmu/aarch64_mmu_descriptor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t aarch64_mmu_table_descriptor(void *pa, struct aarch64_mmu_attr const *attr)
{
    uint64_t desc;

    desc = DESC_TABLE;
    desc |= (uint64_t)pa;
    desc |= (uint64_t)attr->nstable << 63;
    desc |= (uint64_t)attr->aptable << 61;
    desc |= (uint64_t)attr->xntable << 60;
    desc |= (uint64_t)attr->pxntable << 59;

    return desc;
}

static uint64_t block_page_descriptor(void *pa, struct aarch64_mmu_attr const *attr, uint64_t desc)
{
    desc |= (uint64_t)pa;
    desc |= (uint64_t)attr->xn << 54;
    desc |= (uint64_t)attr->pxn << 53;
    desc |= (uint64_t)attr->ng << 11;
    desc |= (uint64_t)attr->af << 10;
    desc |= (uint64_t)attr->sh << 8;
    desc |= (uint64_t)attr->ap21 << 6;
    desc |= (uint64_t)attr->ns << 5;
    desc |= (uint64_t)attr->attrindx << 2;

    return desc;
}

uint64_t aarch64_mmu_block_descriptor(void *pa, struct aarch64_mmu_attr const *attr)
{
    uint64_t desc;

    desc = block_page_descriptor(pa, attr, DESC_BLOCK);

    return desc;
}

uint64_t aarch64_mmu_page_descriptor(void *pa, struct aarch64_mmu_attr const *attr)
{
    uint64_t desc;

    desc = block_page_descriptor(pa, attr, DESC_PAGE);

    return desc;
}

