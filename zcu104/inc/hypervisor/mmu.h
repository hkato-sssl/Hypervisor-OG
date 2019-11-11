/*
 * hypervisor/mmu.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_MMU_H
#define HYPERVISOR_MMU_H

/* includes */

#include "driver/aarch64/system_register/mair.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

#define HYP_MMU_ATTRS   (MAIR_ATTR(0, MAIR_ATTR_NC) |\
                         MAIR_ATTR(1, MAIR_ATTR_WB) |\
                         MAIR_ATTR(2, MAIR_ATTR_WBWA) |\
                         MAIR_ATTR(3, MAIR_ATTR_NC) |\
                         MAIR_ATTR(4, MAIRT_ATTR_DEVICE_nGnRnE) |\
                         MAIR_ATTR(5, MAIRT_ATTR_DEVICE_nGnRE) |\
                         MAIR_ATTR(6, MAIRT_ATTR_DEVICE_nGRE) |\
                         MAIR_ATTR(7, MAIRT_ATTR_DEVICE_GRE))

#define HYP_MMU_NC              0
#define HYP_MMU_WB              1
#define HYP_MMU_WBWA            2
#define HYP_MMU_DEVICE_nGnRnE   4
#define HYP_MMU_DEVICE_nGnRE    5
#define HYP_MMU_DEVICE_nGRE     6
#define HYP_MMU_DEVICE_GRE      7

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_MMU_H */

