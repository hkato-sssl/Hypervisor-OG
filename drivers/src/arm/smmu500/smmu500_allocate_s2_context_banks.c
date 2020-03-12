/*
 * arm/smmu500/smmu500_allocate_s2_context_banks.c
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

static errno_t allocate(uint8_t *idx, struct smmu500 *smmu)
{
    errno_t ret;
    size_t sz;
    uint32_t no;
    uint32_t nr_context_banks;

    nr_context_banks = smmu->nr_context_banks - smmu->nr_s2_context_banks;
    sz = (nr_context_banks + 7) / 8;
    ret = bitmap_search_and_set(&no, smmu->allocate.context_banks, sz);
    if (ret == SUCCESS) {
        if (no < nr_context_banks) {
            *idx = (uint8_t)(no + smmu->nr_s2_context_banks);
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

static errno_t allocate_s2(uint8_t *idx, struct smmu500 *smmu)
{
    errno_t ret;
    size_t sz;
    uint32_t no;

    sz = (smmu->nr_s2_context_banks + 7) / 8;
    ret = bitmap_search_and_set(&no, smmu->allocate.context_banks, sz);
    if ((ret == SUCCESS) && (no < smmu->nr_s2_context_banks)) {
        *idx = (uint8_t)no;
    } else {
        ret = allocate(idx, smmu);
    }

    return ret;
}

errno_t smmu500_allocate_s2_context_banks(uint8_t *idx, struct smmu500 *smmu)
{
    errno_t ret;

    smmu500_lock(smmu);

    if (smmu->nr_s2_context_banks > 0) {
        ret = allocate_s2(idx, smmu);
    } else {
        ret = allocate(idx, smmu);
    }

    smmu500_unlock(smmu);

    return ret;
}

