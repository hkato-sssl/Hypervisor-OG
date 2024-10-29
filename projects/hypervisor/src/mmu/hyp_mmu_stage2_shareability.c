// SPDX-License-Identifier: Apache-2.0
/*
 * mmu/hyp_mmu_stage2_shareability.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/stage2.h"
#include "hypervisor/mmu.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hyp_mmu_stage2_shareability(uint8_t *stage2_shareability,
                                    uint8_t shareability)
{
    errno_t ret;

    ret = SUCCESS;
    switch (shareability) {
    case HYP_MMU_SH_NSH:
        *stage2_shareability = STAGE2_SH_NSH;
        break;
    case HYP_MMU_SH_ISH:
        *stage2_shareability = STAGE2_SH_ISH;
        break;
    case HYP_MMU_SH_OSH:
        *stage2_shareability = STAGE2_SH_OSH;
        break;
    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}
