/*
 * driver/aarch64/mmu_base.h
 *
 * (C) 2019 Hidekazu Kato
 */

/*
 * 制約事項：
 * ・ARMv8仕様対応
 * ・4KB granuleのみ対応
 */

#ifndef DRIVER_AARCH64_MMU_BASE_H
#define DRIVER_AARCH64_MMU_BASE_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/list.h"
#include "lib/system/errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define MEM_4KB                 (4UL * 1024)
#define MEM_64KB                (64UL * 1024)
#define MEM_2MB                 (2UL * 1024 * 1024)
#define MEM_32MB                (32UL * 1024 * 1024)
#define MEM_1GB                 (1UL * 1024 * 1024 * 1024)
#define MEM_16GB                (16UL * 1024 * 1024 * 1024)

#define MMU_DESC_TABLE          3
#define MMU_DESC_BLOCK          1
#define MMU_DESC_PAGE           3
#define MMU_DESC_CONTIGUOUS     BIT(52)

#define MMU_BLOCK_SZ            MEM_4KB

/* types */

enum aarch64_mmu_type { AARCH64_MMU_STAGE2, AARCH64_MMU_EL0, AARCH64_MMU_EL1, AARCH64_MMU_EL2, AARCH64_MMU_EL3 };
enum aarch64_mmu_granule { AARCH64_MMU_4KB_GRANULE, AARCH64_MMU_16KB_GRANULE, AARCH64_MMU_64KB_GRANULE }; 

struct aarch64_mmu_block_pool {
    size_t              block_sz;
    struct list         block_list;
    struct {
        void            *addr;
        size_t          size;
    } block_region;
    struct {
        struct {
            uint64_t    success;
            uint64_t    failure;
        } calloc;
        struct {
            uint64_t    success;
            uint64_t    failure;
        } free;
    } counter;
};

struct aarch64_mmu_block_pool_configuration {
    size_t          block_sz;
    struct {
        void        *addr;
        size_t      size;
    } block_region;
};

typedef uint64_t (*aarch64_mmu_desc_func_t)(void *pa, const void *attr);

struct aarch64_mmu_ops {
    aarch64_mmu_desc_func_t table_descriptor;
    aarch64_mmu_desc_func_t block_descriptor;
    aarch64_mmu_desc_func_t page_descriptor;
};

struct aarch64_mmu_base {
    enum aarch64_mmu_type           type;
    enum aarch64_mmu_granule        granule;
    uint8_t                         start_level;
    uint64_t                        *addr;
    struct aarch64_mmu_block_pool   *pool;
    const struct aarch64_mmu_ops    *ops;
};

struct aarch64_mmu_base_configuration {
    enum aarch64_mmu_type           type;
    enum aarch64_mmu_granule        granule;
    struct aarch64_mmu_block_pool   *pool;
};

/* variables */

/* functions */

errno_t aarch64_mmu_block_pool_initialize(struct aarch64_mmu_block_pool *pool, const struct aarch64_mmu_block_pool_configuration *config);
void *aarch64_mmu_block_calloc(struct aarch64_mmu_block_pool *pool, size_t block_sz);
errno_t aarch64_mmu_block_free(struct aarch64_mmu_block_pool *pool, void *block, size_t block_sz);

errno_t aarch64_mmu_map_4KB_granule(struct aarch64_mmu_base *mmu, void *va, void *pa, size_t sz, const void *attr);
errno_t aarch64_mmu_map_single_region(struct aarch64_mmu_base *mmu, void *va, void *pa, size_t sz, const void *attr, uint32_t level);
errno_t aarch64_mmu_map_contiguous_region(struct aarch64_mmu_base *mmu, void *va, void *pa, size_t sz, const void *attr, uint32_t level);

void aarch64_mmu_write_descriptor(uint64_t *addr, uint64_t desc);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_MMU_BASE_H */

