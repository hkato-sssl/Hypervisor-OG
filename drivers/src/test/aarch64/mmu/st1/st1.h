/*
 * test/aarch64/mmu/st1/st1.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef TEST_AARCH64_MMU_ST1_ST1_H
#define TEST_AARCH64_MMU_ST1_ST1_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/system_register/mair_elx.h"

/* defines */

#define TEST_ATTRS  (MAIR_ATTR(0, MAIR_ATTR_NORMAL_NC) |\
                     MAIR_ATTR(1, MAIR_ATTR_NORMAL_WB) |\
                     MAIR_ATTR(2, MAIR_ATTR_NORMAL_WBWA) |\
                     MAIR_ATTR(3, MAIR_ATTR_NORMAL_WT) |\
                     MAIR_ATTR(4, MAIR_ATTR_NORMAL_WTWA) |\
                     MAIR_ATTR(5, MAIR_ATTR_DEVICE_nGnRnE))

#define MMU_ATTR_NC                     0
#define MMU_ATTR_WB                     1
#define MMU_ATTR_WBWA                   2
#define MMU_ATTR_WT                     3
#define MMU_ATTR_WTWA                   4
#define MMU_ATTR_DEVICE_nGnRnE          5

/* types */

/* variables */

/* functions */

errno_t test_aarch64_mmu_st1_init_memory_map(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* TEST_AARCH64_MMU_ST1_ST1_H */

