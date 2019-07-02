/*
 * aarch64/mmu/mmu_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef AARCH64_MMU_MMU_LOCAL_H
#define AARCH64_MMU_MMU_LOCAL_H

/* includes */

/* defines */

#define MEM_4KB         (4UL * 1024)
#define MEM_2MB         (2UL * 1024 * 1024)
#define MEM_1GB         (1UL * 1024 * 1024 * 1024)

#define MMU_BLOCK_SZ    MEM_4KB

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

errno_t aarch64_mmu_set_tt(struct aarch64_mmu_trans_table *tt);
errno_t aarch64_mmu_enable_asm(struct aarch64_mmu_trans_table const *tt);
void aarch64_mmu_write_tt(uint64_t *addr, uint64_t desc);
void aarch64_mmu_tlbi_va(void *va);
void aarch64_mmu_tlbi_all(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* AARCH64_MMU_MMU_LOCAL_H */

