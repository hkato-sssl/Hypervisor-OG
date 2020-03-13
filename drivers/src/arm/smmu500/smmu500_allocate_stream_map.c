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

errno_t smmu500_allocate_stream_map(uint8_t *idx, struct smmu500 *smmu)
{
    errno_t ret;
    uint32_t no;
    size_t sz;

    sz = (smmu->nr_stream_maps + 7) / 8;
    ret = bitmap_search_and_set(&no, smmu->allocation.stream_maps, sz, 0);
    if (ret == SUCCESS) {
        if (no < smmu->nr_stream_maps) {
            *idx = (uint8_t)no;
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

