// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/stage2/stage2_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef AARCH64_STAGE2_STAGE2_LOCAL_H
#define AARCH64_STAGE2_STAGE2_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include "driver/aarch64/mmu_base.h"
#include "lib/system/errno.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
extern "C" {
#endif

uint64_t
aarch64_stage2_table_descriptor(void *pa,
                                const struct aarch64_stage2_attr *attr);
uint64_t
aarch64_stage2_block_descriptor(void *pa,
                                const struct aarch64_stage2_attr *attr);
uint64_t aarch64_stage2_page_descriptor(void *pa,
                                        const struct aarch64_stage2_attr *attr);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* AARCH64_STAGE2_STAGE2_LOCAL_H */
