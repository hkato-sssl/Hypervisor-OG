/*
 * aarch64/mmu/aarch64_mmu_block_pool.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu.h"
#include "lib/list.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "mmu_local.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t
validate_parameters(struct aarch64_mmu_block_pool *pool,
                    const struct aarch64_mmu_block_pool_configuration *config)
{
    errno_t ret;

    if ((pool != NULL) && (config != NULL)
        && (config->block_region.addr != NULL)
        && (IS_ALIGNED((uintptr_t)(config->block_region.addr), 4096))
        && (config->block_sz == 4096) && (config->block_region.size > 0)
        && (IS_ALIGNED((uintptr_t)(config->block_region.size), 4096))) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_block_pool_initialize(
    struct aarch64_mmu_block_pool *pool,
    const struct aarch64_mmu_block_pool_configuration *config)
{
    errno_t ret;
    size_t n;
    char *p;
    struct list_element *e;

    memset(pool, 0, sizeof(*pool));

    spin_lock_init(&(pool->lock));
    pool->block_sz = config->block_sz;
    pool->block_region.addr = config->block_region.addr;
    pool->block_region.size = config->block_region.size;

    ret = SUCCESS;
    list_init(&(pool->block_list));
    p = pool->block_region.addr;
    for (n = pool->block_region.size / 4096; n > 0; --n) {
        e = (struct list_element *)p;
        e->value = p;
        ret = list_append(&(pool->block_list), e);
        if (ret != SUCCESS) {
            break;
        }
        p += 4096;
    }

    return ret;
}

errno_t aarch64_mmu_block_pool_initialize(
    struct aarch64_mmu_block_pool *pool,
    const struct aarch64_mmu_block_pool_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(pool, config);
    if (ret == SUCCESS) {
        ret = mmu_block_pool_initialize(pool, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

void *aarch64_mmu_block_calloc(struct aarch64_mmu_block_pool *pool,
                               size_t block_sz)
{
    void *p;

    spin_lock(&(pool->lock));

    if ((pool != NULL) && (pool->block_sz == block_sz)) {
        p = list_get_front(&(pool->block_list));
        if (p != NULL) {
            memset(p, 0, block_sz);
            aarch64_dcache_clean_range(p, block_sz);
            ++(pool->counter.calloc.success);
        } else {
            ++(pool->counter.calloc.failure);
        }
    } else {
        ++pool->counter.calloc.failure;
        p = NULL;
    }

    spin_unlock(&(pool->lock));

    return p;
}

errno_t aarch64_mmu_block_free(struct aarch64_mmu_block_pool *pool, void *block,
                               size_t block_sz)
{
    errno_t ret;

    spin_lock(&(pool->lock));

    if ((pool != NULL) && (block != NULL) && (pool->block_sz == block_sz)) {
        ret = list_append(&(pool->block_list), block);
        if (ret == SUCCESS) {
            ++(pool->counter.free.success);
        } else {
            ++(pool->counter.free.failure);
        }
    } else {
        ++(pool->counter.free.failure);
        ret = -EINVAL;
    }

    spin_unlock(&(pool->lock));

    return ret;
}
