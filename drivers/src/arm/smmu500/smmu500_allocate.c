/*
 * arm/smmu500/smmu500_allocate.c
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

static errno_t alloc(uint8_t *id, void *map, size_t map_size, uint32_t nr_bits)
{
    errno_t ret;
    uint32_t no;

    ret = bitmap_search0(&no, map, map_size);
    if (ret == SUCCESS) {
        if (no < nr_bits) {
            *id = (uint8_t)no;
            ret = bitmap_set(map, map_size, no);
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t smmu500_allocate(uint8_t *id, struct smmu500 *smmu, void *map, size_t map_size, uint32_t nr_bits)
{
    errno_t ret;

    spin_lock(&(smmu->lock));
    ret = alloc(id, map, map_size, nr_bits);
    spin_unlock(&(smmu->lock));

    return ret;
}

