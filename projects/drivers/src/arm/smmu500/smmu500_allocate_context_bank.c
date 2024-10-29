// SPDX-License-Identifier: Apache-2.0
/*
 * arm/smmu500/smmu500_allocate_context_bank.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/smmu500.h"
#include "lib/bit.h"
#include "lib/bitmap.h"
#include "lib/system/errno.h"
#include "smmu500_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t allocate_context_bank(struct smmu500 *smmu, uint8_t *cb,
                                     uint32_t start_bit)
{
    errno_t ret;
    size_t sz;
    uint32_t no;

    smmu500_lock(smmu);

    sz = ALIGN(smmu->nr_context_banks, 8);
    ret = bitmap_search_and_set(&no, smmu->allocation.context_banks, sz,
                                start_bit);
    if (ret == SUCCESS) {
        if (no < smmu->nr_context_banks) {
            *cb = (uint8_t)no;
        } else {
            ret = -ENOMEM;
        }
    }

    smmu500_unlock(smmu);

    return ret;
}

errno_t smmu500_allocate_context_bank(struct smmu500 *smmu, uint8_t *cb)
{
    errno_t ret;

    ret = allocate_context_bank(smmu, cb, smmu->nr_s2_context_banks);

    return ret;
}

errno_t smmu500_allocate_s2_context_bank(struct smmu500 *smmu, uint8_t *cb)
{
    errno_t ret;

    ret = allocate_context_bank(smmu, cb, 0);

    return ret;
}
