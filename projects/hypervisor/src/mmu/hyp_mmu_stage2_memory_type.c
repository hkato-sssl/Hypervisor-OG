/*
 * mmu/hyp_mmu_stage2_memory_type.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/mmu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hyp_mmu_stage2_memory_type(uint8_t *stage2_memory_type, uint8_t memory_type)
{
    errno_t ret;

    ret = SUCCESS;
    switch (memory_type) {
    case HYP_MMU_MT_NORMAL_NC:
        *stage2_memory_type = STAGE2_MEMATTR_NORMAL_NC;
        break;
    case HYP_MMU_MT_NORMAL_WB:
        *stage2_memory_type = STAGE2_MEMATTR_NORMAL_WB;
        break;
    case HYP_MMU_MT_NORMAL_WBWA:
        *stage2_memory_type = STAGE2_MEMATTR_NORMAL_WB;
        break;
    case HYP_MMU_MT_DEVICE_nGnRnE:
        *stage2_memory_type = STAGE2_MEMATTR_DEVICE_nGnRnE;
        break;
    case HYP_MMU_MT_DEVICE_nGnRE:
        *stage2_memory_type = STAGE2_MEMATTR_DEVICE_nGnRE;
        break;
    case HYP_MMU_MT_DEVICE_nGRE:
        *stage2_memory_type = STAGE2_MEMATTR_DEVICE_nGRE;
        break;
    case HYP_MMU_MT_DEVICE_GRE:
        *stage2_memory_type = STAGE2_MEMATTR_DEVICE_GRE;
        break;
    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}

