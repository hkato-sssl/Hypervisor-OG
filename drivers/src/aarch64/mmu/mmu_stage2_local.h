/*
 * aarch64/mmu/mmu_stage2_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef AARCH64_MMU_MMU_STAGE2_LOCAL_H
#define AARCH64_MMU_MMU_STAGE2_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
extern "C" {
#endif

uint64_t aarch64_mmu_stage2_table_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr);
uint64_t aarch64_mmu_stage2_block_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr);
uint64_t aarch64_mmu_stage2_page_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* AARCH64_MMU_MMU_STAGE2_LOCAL_H */
