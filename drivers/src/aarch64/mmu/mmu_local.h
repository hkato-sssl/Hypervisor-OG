/*
 * aarch64/mmu/mmu_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef AARCH64_MMU_MMU_LOCAL_H
#define AARCH64_MMU_MMU_LOCAL_H

/* includes */

/* defines */

#define MMU_BLOCK_SZ    4096

#ifndef ASSEMBLY

/* includes */

#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
extern "C" {
#endif

void aarch64_mmu_clear_block(void *addr, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* AARCH64_MMU_MMU_LOCAL_H */

