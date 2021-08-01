/*
 * system/mmu.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef SYSTEM_MMU_H
#define SYSTEM_MMU_H

/* includes */

#include "driver/aarch64/system_register/mair_elx.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

#define EL1_MMU_ATTRS   (MAIR_ATTR(0, MAIR_ATTR_NORMAL_NC) |\
                         MAIR_ATTR(1, MAIR_ATTR_NORMAL_WB) |\
                         MAIR_ATTR(2, MAIR_ATTR_NORMAL_WBWA) |\
                         MAIR_ATTR(3, MAIR_ATTR_NORMAL_WT) |\
                         MAIR_ATTR(4, MAIR_ATTR_DEVICE_nGnRnE) |\
                         MAIR_ATTR(5, MAIR_ATTR_DEVICE_nGnRE) |\
                         MAIR_ATTR(6, MAIR_ATTR_DEVICE_nGRE) |\
                         MAIR_ATTR(7, MAIR_ATTR_DEVICE_GRE))

#define EL1_MMU_NC              0
#define EL1_MMU_WB              1
#define EL1_MMU_WBWA            2
#define EL1_MMU_DEVICE_nGnRnE   4
#define EL1_MMU_DEVICE_nGnRE    5
#define EL1_MMU_DEVICE_nGRE     6
#define EL1_MMU_DEVICE_GRE      7

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SYSTEM_MMU_H */

