// SPDX-License-Identifier: Apache-2.0
/*
 * arm/smmu500/smmu500_free_context_bank.c
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

errno_t smmu500_free_context_bank(struct smmu500 *smmu, uint8_t idx)
{
    errno_t ret;
    size_t sz;

    if ((smmu != NULL) && (idx < smmu->nr_context_banks)) {
        sz = ALIGN(smmu->nr_context_banks, 8) / 8;
        ret = bitmap_clear(smmu->allocation.context_banks, sz, idx);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
