/*
 * driver/test/aarch64/mmu/st2/st2.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_TEST_AARCH64_MMU_ST2_ST2_H
#define DRIVER_TEST_AARCH64_MMU_ST2_ST2_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "driver/aarch64/stage2.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

errno_t test_aarch64_stage2_init_memory_map(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_TEST_AARCH64_MMU_ST2_ST2_H */
