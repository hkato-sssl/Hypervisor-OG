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

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

#define MEM_4KB         (4UL * 1024)
#define MEM_64KB        (64UL * 1024)
#define MEM_2MB         (2UL * 1024 * 1024)
#define MEM_32MB        (32UL * 1024 * 1024)
#define MEM_1GB         (1UL * 1024 * 1024 * 1024)
#define MEM_16GB        (16UL * 1024 * 1024 * 1024)

#define DESC_TABLE      3
#define DESC_BLOCK      1
#define DESC_PAGE       3

#define MMU_BLOCK_SZ    MEM_4KB

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
extern "C" {
#endif

errno_t aarch64_mmu_map_4KB_granule(struct aarch64_mmu *mmu, void *va, void *pa, size_t sz, void const *attr);
uint64_t aarch64_mmu_table_descriptor(void *pa, struct aarch64_mmu_attr const *attr);
uint64_t aarch64_mmu_block_descriptor(void *pa, struct aarch64_mmu_attr const *attr);
uint64_t aarch64_mmu_page_descriptor(void *pa, struct aarch64_mmu_attr const *attr);

uint64_t aarch64_mmu_stage2_table_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr);
uint64_t aarch64_mmu_stage2_block_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr);
uint64_t aarch64_mmu_stage2_page_descriptor(void *pa, struct aarch64_mmu_stage2_attr const *attr);

errno_t aarch64_mmu_set_translation_table(struct aarch64_mmu *mmu);
errno_t aarch64_mmu_enable_asm(void);
void aarch64_mmu_tlbi_va(void *va);
void aarch64_mmu_tlbi_all(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* AARCH64_MMU_MMU_LOCAL_H */

