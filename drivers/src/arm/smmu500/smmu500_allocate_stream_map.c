/*
 * arm/smmu500/smmu500_allocate_stream_map.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/bitmap.h"
#include "lib/aarch64.h"
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
    uint32_t no;
    size_t sz;

    sz = (smmu->nr_stream_maps + 7) / 8;
    ret = bitmap_search_and_set(&no, smmu->allocate.stream_maps, sz);
    if (ret == SUCCESS) {
        if (no < smmu->nr_stream_maps) {
            *idx = (uint8_t)no;
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t smmu500_allocate_stream_map(uint8_t *idx, struct smmu500 *smmu)
{
    errno_t ret;

    smmu500_lock(smmu);
    ret = allocate(idx, smmu);
    smmu500_unlock(smmu);

    return ret;
}

