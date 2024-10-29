// SPDX-License-Identifier: Apache-2.0
/*
 * driver/aarch64/mmu/aarch64_mmu_enable.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/mmu.h"
#include "driver/aarch64/system_register.h"
#include "lib/system/errno.h"
#include "mmu_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t mmu_enable(struct aarch64_mmu *mmu)
{
    errno_t ret;

    ret = aarch64_mmu_set_translation_table(mmu);
    if (ret == SUCCESS) {
        ret = aarch64_mmu_enable_asm();
    }

    return ret;
}

errno_t aarch64_mmu_enable(struct aarch64_mmu *mmu)
{
    errno_t ret;
    uint64_t d;

    if (mmu != NULL) {
        d = aarch64_read_sctlr();
        if ((d & SCTLR_M) == 0) {
            ret = mmu_enable(mmu);
        } else {
            ret = -EBUSY;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}
