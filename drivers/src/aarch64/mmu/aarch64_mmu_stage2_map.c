/*
 * driver/aarch64/mmu/aarch64_mmu_stage2_map.c
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

errno_t aarch64_mmu_stage2_map(struct aarch64_mmu_trans_table *tt, void *va, void *pa, size_t sz, struct aarch64_mmu_stage2_attr const *attr)
{
    errno_t ret;
    union mmu_attr attribute;

    /* Support 4KB granule only */
    if ((tt->stage == AARCH64_MMU_STAGE2) && (tt->granule == AARCH64_MMU_4KB_GRANULE)) {
        attribute.stage2 = attr;
        ret = aarch64_mmu_map_4KB_granule(tt, va, pa, sz, &attribute);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
