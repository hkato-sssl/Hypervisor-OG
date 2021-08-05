/*
 * hypervisor/mmu.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_MMU_H
#define HYPERVISOR_MMU_H

/* includes */

#include "driver/aarch64/system_register/mair_elx.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

#define HYP_MMU_ATTRS                                                          \
    (MAIR_ATTR(0, MAIR_ATTR_NORMAL_NC) | MAIR_ATTR(1, MAIR_ATTR_NORMAL_WB)     \
     | MAIR_ATTR(2, MAIR_ATTR_NORMAL_WBWA) | MAIR_ATTR(3, MAIR_ATTR_NORMAL_NC) \
     | MAIR_ATTR(4, MAIR_ATTR_DEVICE_nGnRnE)                                   \
     | MAIR_ATTR(5, MAIR_ATTR_DEVICE_nGnRE)                                    \
     | MAIR_ATTR(6, MAIR_ATTR_DEVICE_nGRE)                                     \
     | MAIR_ATTR(7, MAIR_ATTR_DEVICE_GRE))

#define HYP_MMU_MT_NORMAL_NC     0
#define HYP_MMU_MT_NORMAL_WB     1
#define HYP_MMU_MT_NORMAL_WBWA   2
#define HYP_MMU_MT_DEVICE_nGnRnE 4
#define HYP_MMU_MT_DEVICE_nGnRE  5
#define HYP_MMU_MT_DEVICE_nGRE   6
#define HYP_MMU_MT_DEVICE_GRE    7

#define HYP_MMU_SH_NSH           0
#define HYP_MMU_SH_ISH           1
#define HYP_MMU_SH_OSH           2

/* types */

/* variables */

/* functions */

errno_t hyp_mmu_stage2_memory_type(uint8_t *stage2_memory_type,
                                   uint8_t memory_type);
errno_t hyp_mmu_stage2_shareability(uint8_t *stage2_shareability,
                                    uint8_t shareability);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_MMU_H */
