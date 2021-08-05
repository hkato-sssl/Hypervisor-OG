/*
 * arm/smmu500/smmu500_allocate_stream_match_register.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/smmu500.h"
#include "lib/aarch64.h"
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

errno_t smmu500_allocate_stream_match_register(struct smmu500 *smmu,
                                               uint8_t *id)
{
    errno_t ret;
    uint32_t no;
    size_t sz;

    smmu500_lock(smmu);

    sz = (smmu->nr_stream_matches + 7) / 8;
    ret = bitmap_search_and_set(&no, smmu->allocation.stream_matches, sz, 0);
    if (ret == SUCCESS) {
        if (no < smmu->nr_stream_matches) {
            *id = (uint8_t)no;
        } else {
            ret = -ENOMEM;
        }
    }

    smmu500_unlock(smmu);

    return ret;
}
