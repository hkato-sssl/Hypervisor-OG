/*
 * arm/smmu500/smmu500_free_context_bank.c
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

static errno_t free_context_bank(struct smmu500 *smmu, uint8_t id)
{
    errno_t ret;
    size_t sz;

    id -= smmu->nr_s2_context_banks;
    sz = (smmu->nr_context_banks - smmu->nr_s2_context_banks + 7) / 8;
    ret = bitmap_clear(smmu->allocate.context_banks, sz, id);

    return ret;
}

static errno_t free_s2_context_bank(struct smmu500 *smmu, uint8_t id)
{
    errno_t ret;
    size_t sz;

    sz = (smmu->nr_s2_context_banks + 7) / 8;
    ret = bitmap_clear(smmu->allocate.s2_context_banks, sz, id);

    return ret;
}

errno_t smmu500_free_context_bank(struct smmu500 *smmu, uint8_t id)
{
    errno_t ret;

    if (id < smmu->nr_context_banks) {
        smmu500_lock(smmu);
        if (id < smmu->nr_s2_context_banks) {
            ret = free_s2_context_bank(smmu, id);
        } else {
            ret = free_context_bank(smmu, id);
        }
        smmu500_unlock(smmu);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

