/*
 * driver/aarch64/mmu/aarch64_mmu_stage2_map.c
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

errno_t aarch64_mmu_stage2_map(struct aarch64_mmu_stage2 *mmu, void *va, void *pa, size_t sz, struct aarch64_mmu_stage2_attr const *attr)
{
    errno_t ret;

    /* Support 4KB granule only */
    if ((mmu->base.type == AARCH64_MMU_STAGE2) && (mmu->base.granule == AARCH64_MMU_4KB_GRANULE)) {
        ret = aarch64_mmu_map_4KB_granule(&(mmu->base), va, pa, sz, attr);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

