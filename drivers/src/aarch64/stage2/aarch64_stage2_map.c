/*
 * driver/aarch64/mmu/aarch64_stage2_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/stage2.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t aarch64_stage2_map(struct aarch64_stage2 *st2, void *va, void *pa, size_t sz, const struct aarch64_stage2_attr *attr)
{
    errno_t ret;

    /* Support 4KB granule only */
    if ((st2->base.type == AARCH64_MMU_STAGE2) && (st2->base.granule == AARCH64_MMU_4KB_GRANULE)) {
        ret = aarch64_mmu_map_4KB_granule(&(st2->base), va, pa, sz, attr);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

