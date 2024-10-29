// SPDX-License-Identifier: Apache-2.0
/*
 * driver/test/aarch64/mmu/cache_test.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_TEST_AARCH64_CACHE_TEST_H
#define DRIVER_TEST_AARCH64_CACHE_TEST_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "driver/aarch64/system_register/mair_elx.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

#define TEST_ATTRS                                                             \
    (MAIR_ATTR(0, MAIR_ATTR_NORMAL_NC) | MAIR_ATTR(1, MAIR_ATTR_NORMAL_WB)     \
     | MAIR_ATTR(2, MAIR_ATTR_NORMAL_WBWA) | MAIR_ATTR(3, MAIR_ATTR_NORMAL_WT) \
     | MAIR_ATTR(4, MAIR_ATTR_NORMAL_WTWA)                                     \
     | MAIR_ATTR(5, MAIR_ATTR_DEVICE_nGnRnE))

#define MMU_ATTR_NC            0
#define MMU_ATTR_WB            1
#define MMU_ATTR_WBWA          2
#define MMU_ATTR_WT            3
#define MMU_ATTR_WTWA          4
#define MMU_ATTR_DEVICE_nGnRnE 5

/* types */

/* variables */

/* functions */

errno_t test_aarch64_cache_init_memory_map(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_TEST_AARCH64_CACHE_TEST_H */
