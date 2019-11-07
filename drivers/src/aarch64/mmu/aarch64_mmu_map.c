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

errno_t aarch64_mmu_map(struct aarch64_mmu *mmu, void *va, void *pa, size_t sz, struct aarch64_mmu_attr const *attr)
{
    errno_t ret;
    union mmu_attr attribute;

    /* Support 4KB granule only */
    if ((mmu->stage == AARCH64_MMU_STAGE1) && (mmu->granule == AARCH64_MMU_4KB_GRANULE)) {
        attribute.stage1 = attr;
        ret = aarch64_mmu_map_4KB_granule(mmu, va, pa, sz, &attribute);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
