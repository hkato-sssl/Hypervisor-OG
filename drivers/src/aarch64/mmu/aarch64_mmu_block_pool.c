/*
 * aarch64/mmu/aarch64_mmu_block_pool.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/atomic.h"
#include "lib/list.h"
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t validate_parameters(struct aarch64_mmu_block_pool *pool, struct aarch64_mmu_block_pool_configure const *conf)
{
    errno_t ret;

    if ((pool != NULL) && (conf != NULL) &&
        (conf->block_region.addr != NULL) &&
        (IS_ALIGNED((uintptr_t)(conf->block_region.addr), 4096)) &&
        (conf->block_sz == 4096) &&
        (conf->block_region.size > 0) &&
        (IS_ALIGNED((uintptr_t)(conf->block_region.size), 4096))) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_block_pool_init(struct aarch64_mmu_block_pool *pool, struct aarch64_mmu_block_pool_configure const *conf)
{
    errno_t ret;
    size_t n;
    char *p;
    struct list_element *e;

    memset(pool, 0, sizeof(*pool));

    pool->block_sz = conf->block_sz;
    pool->block_region.addr = conf->block_region.addr;
    pool->block_region.size = conf->block_region.size;

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

    ret = validate_parameters(pool, conf);
    if (ret == SUCCESS) {
        ret = mmu_block_pool_init(pool, conf);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

void *aarch64_mmu_block_calloc(struct aarch64_mmu_block_pool *pool, size_t block_sz)
{
    void *p;

    if ((pool != NULL) && (pool->block_sz == block_sz)) {
        p = list_get_front(&(pool->block_list));
        if (p != NULL) {
            memset(p, 0, block_sz);
            atomic_inc_u64(&(pool->counter.calloc.success));
        } else {
            atomic_inc_u64(&(pool->counter.calloc.failure));
        }
    } else {
        atomic_inc_u64(&(pool->counter.calloc.failure));
        p = NULL;
    }

    return p;
}

errno_t aarch64_mmu_block_free(struct aarch64_mmu_block_pool *pool, void *block, size_t block_sz)
{
    errno_t ret;

    if ((pool != NULL) && (block != NULL) && (pool->block_sz == block_sz)) {
        ret = list_append(&(pool->block_list), block);
        if (ret == SUCCESS) {
            atomic_inc_u64(&(pool->counter.free.success));
        } else {
            atomic_inc_u64(&(pool->counter.free.failure));
        }
    } else {
        atomic_inc_u64(&(pool->counter.free.failure));
        ret = -EINVAL;
    }

    return ret;
}

