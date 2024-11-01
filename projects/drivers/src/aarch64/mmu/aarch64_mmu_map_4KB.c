// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/mmu/aarch64_mmu_map_4KB.c - mapping function with 4KB granule
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu_base.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t mmu_map(struct aarch64_mmu_base *mmu, void **va, void **pa,
                       size_t *sz, const void *attr)
{
    errno_t ret;

    if ((*sz >= MEM_1GB)
        && ((ret =
                 aarch64_mmu_map_single_region(mmu, *va, *pa, MEM_1GB, attr, 1))
            == SUCCESS)) {
        *(char **)va += MEM_1GB;
        *(char **)pa += MEM_1GB;
        *sz -= MEM_1GB;
    } else if ((*sz >= MEM_2MB)
               && ((ret = aarch64_mmu_map_single_region(mmu, *va, *pa, MEM_2MB,
                                                        attr, 2))
                   == SUCCESS)) {
        *(char **)va += MEM_2MB;
        *(char **)pa += MEM_2MB;
        *sz -= MEM_2MB;
    } else if ((*sz >= MEM_64KB)
               && ((ret = aarch64_mmu_map_contiguous_region(mmu, *va, *pa,
                                                            MEM_64KB, attr, 3))
                   == SUCCESS)) {
        *(char **)va += MEM_64KB;
        *(char **)pa += MEM_64KB;
        *sz -= MEM_64KB;
    } else if ((*sz >= MEM_4KB)
               && ((ret = aarch64_mmu_map_single_region(mmu, *va, *pa, MEM_4KB,
                                                        attr, 3))
                   == SUCCESS)) {
        *(char **)va += MEM_4KB;
        *(char **)pa += MEM_4KB;
        *sz -= MEM_4KB;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_map_4KB_granule(struct aarch64_mmu_base *mmu, void *va,
                                    void *pa, size_t sz, const void *attr)
{
    errno_t ret;

    aarch64_mmu_base_lock(mmu);

    do {
        ret = mmu_map(mmu, &va, &pa, &sz, attr);
    } while ((ret == SUCCESS) && (sz != 0));

    aarch64_mmu_base_unlock(mmu);

    return ret;
}
