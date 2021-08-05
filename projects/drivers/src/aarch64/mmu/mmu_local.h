/*
 * aarch64/mmu/mmu_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef AARCH64_MMU_MMU_LOCAL_H
#define AARCH64_MMU_MMU_LOCAL_H

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

uint64_t aarch64_mmu_table_descriptor(void *pa,
                                      const struct aarch64_mmu_attr *attr);
uint64_t aarch64_mmu_block_descriptor(void *pa,
                                      const struct aarch64_mmu_attr *attr);
uint64_t aarch64_mmu_page_descriptor(void *pa,
                                     const struct aarch64_mmu_attr *attr);

errno_t aarch64_mmu_set_translation_table(struct aarch64_mmu *mmu);
errno_t aarch64_mmu_enable_asm(void);
void aarch64_mmu_tlbi_va(void *va);
void aarch64_mmu_tlbi_all(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* AARCH64_MMU_MMU_LOCAL_H */
