/*
 * arm/smmu500/smmu500_allocate_context_bank.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/bitmap.h"
#include "lib/system/errno.h"
#include "driver/arm/smmu500.h"
#include "smmu500_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t allocate_context_bank(uint8_t *idx, struct smmu500 *smmu, uint32_t start_bit)
{
    errno_t ret;
    size_t sz;
    uint32_t no;

    sz = ALIGN(smmu->nr_context_banks, 8);
    ret = bitmap_search_and_set(&no, smmu->allocation.context_banks, sz, start_bit);
    if (ret == SUCCESS) {
        if (no < smmu->nr_context_banks) {
            *idx = (uint8_t)no;
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t smmu500_allocate_context_bank(uint8_t *idx, struct smmu500 *smmu)
{
    errno_t ret;

    ret = allocate_context_bank(idx, smmu, smmu->nr_s2_context_banks);

    return ret;
}

errno_t smmu500_allocate_s2_context_bank(uint8_t *idx, struct smmu500 *smmu)
{
    errno_t ret;

    ret = allocate_context_bank(idx, smmu, 0);

    return ret;
}

