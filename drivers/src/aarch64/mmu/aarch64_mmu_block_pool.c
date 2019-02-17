/*
 * aarch64/mmu/aarch64_mmu_block_pool.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/atomic.h"
#include "lib/list.h"
#include "lib/errno.h"
#include "driver/aarch64/mmu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_valid_parameters(struct aarch64_mmu_block_pool *pool, struct aarch64_mmu_block_pool_configure const *conf)
{
    bool valid;

    if ((pool != NULL) && (conf != NULL) &&
        (conf->block_region.addr != NULL) &&
        (IS_ALIGNED(conf->block_region.addr, 4096)) &&
        (conf->block_sz == 4096) &&
        (conf->block_region.size > 0) &&
        (IS_ALIGNED(conf->block_region.size, 4096))) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

static errno_t mmu_block_pool_init(struct aarch64_mmu_block_pool *pool, struct aarch64_mmu_block_pool_configure const *conf)
{
    errno_t ret;
    size_t n;
    char *p;
    struct list_element *e;

    pool->block_sz = conf->block_sz;
    pool->block_region.addr = conf->block_region.addr;
    pool->block_region.size = conf->block_region.size;
    pool->counter.alloc = 0;
    pool->counter.free = 0;

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


errno_t aarch64_mmu_block_pool_init(struct aarch64_mmu_block_pool *pool, struct aarch64_mmu_block_pool_configure const *conf)
{
    errno_t ret;

    if (is_valid_parameters(pool, conf)) {
        ret = mmu_block_pool_init(pool, conf);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

void *aarch64_mmu_block_alloc(struct aarch64_mmu_block_pool *pool, size_t block_sz)
{
    void *p;

    if ((pool != NULL) && (pool->block_sz == block_sz)) {
        atomic_inc_u64(&(pool->counter.alloc));
        p = list_get_front(&(pool->block_list));
    } else {
        p = NULL;
    }

    return p;
}

errno_t aarch64_mmu_block_free(struct aarch64_mmu_block_pool *pool, void *block, size_t block_sz)
{
    errno_t ret;

    if ((pool != NULL) && (block != NULL) && (pool->block_sz == block_sz)) {
        atomic_inc_u64(&(pool->counter.free));
        ret = list_append(&(pool->block_list), block);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

