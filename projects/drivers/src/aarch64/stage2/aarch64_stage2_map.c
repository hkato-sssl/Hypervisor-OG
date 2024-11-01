// SPDX-License-Identifier: Apache-2.0
/*
 * driver/aarch64/mmu/aarch64_stage2_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/stage2.h"
#include "lib/system/errno.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_valid_address(const struct aarch64_stage2 *stage2, void *ipa,
                             void *pa)
{
    bool ret;

    if ((((uintptr_t)ipa & stage2->pa_mask) == 0)
        && (((uintptr_t)pa & stage2->pa_mask) == 0)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static errno_t validate_parameters(const struct aarch64_stage2 *stage2,
                                   void *ipa, void *pa)
{
    errno_t ret;

    /* Support 4KB granule only */
    if (stage2->base.type != AARCH64_MMU_STAGE2) {
        ret = -EINVAL;
    } else if (stage2->base.granule != AARCH64_MMU_4KB_GRANULE) {
        ret = -EINVAL;
    } else if (! is_valid_address(stage2, ipa, pa)) {
        ret = -EFAULT;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t aarch64_stage2_map(struct aarch64_stage2 *stage2, void *ipa, void *pa,
                           size_t sz, const struct aarch64_stage2_attr *attr)
{
    errno_t ret;

    ret = validate_parameters(stage2, ipa, pa);
    if (ret == SUCCESS) {
        ret = aarch64_mmu_map_4KB_granule(&(stage2->base), ipa, pa, sz, attr);
    }

    return ret;
}
