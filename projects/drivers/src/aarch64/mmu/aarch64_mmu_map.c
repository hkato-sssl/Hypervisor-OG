/*
 * driver/aarch64/mmu/aarch64_mmu_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t aarch64_mmu_map(struct aarch64_mmu *mmu, void *va, void *pa, size_t sz, const struct aarch64_mmu_attr *attr)
{
    errno_t ret;

    /* Support 4KB granule only */
    if ((mmu->base.type != AARCH64_MMU_STAGE2) && (mmu->base.granule == AARCH64_MMU_4KB_GRANULE)) {
        ret = aarch64_mmu_map_4KB_granule(&(mmu->base), va, pa, sz, attr);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
