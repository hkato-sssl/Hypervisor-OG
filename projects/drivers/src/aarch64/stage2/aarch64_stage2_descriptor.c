// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/stage2/aarch64_stage2_descriptor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/stage2.h"
#include "stage2_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t aarch64_stage2_table_descriptor(void *pa,
                                         const struct aarch64_stage2_attr *attr)
{
    uint64_t desc;

    desc = (uint64_t)pa | MMU_DESC_TABLE;

    return desc;
}

static uint64_t block_page_descriptor(void *pa,
                                      const struct aarch64_stage2_attr *attr,
                                      uint64_t desc)
{
    desc |= (uint64_t)pa;
    desc |= (uint64_t)attr->xn << 54;
    desc |= (uint64_t)attr->af << 10;
    desc |= (uint64_t)attr->sh << 8;
    desc |= (uint64_t)attr->s2ap << 6;
    desc |= (uint64_t)attr->memattr << 2;

    desc |= (uint64_t)attr->smmu.wacfg << 62;
    desc |= (uint64_t)attr->smmu.racfg << 60;

    return desc;
}

uint64_t aarch64_stage2_block_descriptor(void *pa,
                                         const struct aarch64_stage2_attr *attr)
{
    uint64_t desc;

    desc = block_page_descriptor(pa, attr, MMU_DESC_BLOCK);

    return desc;
}

uint64_t aarch64_stage2_page_descriptor(void *pa,
                                        const struct aarch64_stage2_attr *attr)
{
    uint64_t desc;

    desc = block_page_descriptor(pa, attr, MMU_DESC_PAGE);

    return desc;
}

uint64_t aarch64_stage2_contiguous_block_descriptor(
    void *pa, const struct aarch64_stage2_attr *attr)
{
    uint64_t desc;

    desc = aarch64_stage2_block_descriptor(pa, attr);
    desc |= MMU_DESC_CONTIGUOUS;

    return desc;
}

uint64_t aarch64_stage2_contiguous_page_descriptor(
    void *pa, const struct aarch64_stage2_attr *attr)
{
    uint64_t desc;

    desc = aarch64_stage2_page_descriptor(pa, attr);
    desc |= MMU_DESC_CONTIGUOUS;

    return desc;
}
