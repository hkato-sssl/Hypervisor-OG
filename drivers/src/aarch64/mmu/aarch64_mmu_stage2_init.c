/*
 * aarch64/mmu/aarch64_mmu_stage2stage2_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#if 0
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register/vtcr_el2.h"
#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static struct aarch64_mmu_ops const mmu_ops = {
    (aarch64_mmu_desc_func_t)aarch64_mmu_stage2_table_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_mmu_stage2_block_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_mmu_stage2_page_descriptor
};

/* functions */

static uint64_t create_vtcr(struct aarch64_mmu_configuration const *config)
{
    uint64_t d;

    d = VTCR_EL2_RES1;

    return d;
}

static errno_t validate_parameters(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config)
{
    errno_t ret;

    if ((mmu != NULL) && (config != NULL) && (config->pool.block_sz == MMU_BLOCK_SZ)) {
        if ((config->type == AARCH64_MMU_STAGE2) && (config->granule == AARCH64_MMU_4KB_GRANULE)) {
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }

    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t init(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config)
{
    mmu->active = false;
    mmu->type = config->type;
    mmu->granule = config->granule;
    mmu->ops = &mmu_ops;
    mmu->stage2.vmid = config->stage2.vmid;
    mmu->stage2.vtcr = create_vtcr(config);

    return SUCCESS;
}

static errno_t mmu_stage2_init(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config)
{
    errno_t ret;

    memset(mmu, 0, sizeof(*mmu));

    ret = aarch64_mmu_block_pool_init(&(mmu->pool), &(config->pool));
    if (ret == SUCCESS) {
        mmu->addr = aarch64_mmu_block_calloc(&(mmu->pool), MMU_BLOCK_SZ);
        if (mmu->addr != NULL) {
            ret = init(mmu, config);
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t aarch64_mmu_stage2_init(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config)
{
    errno_t ret;

    ret = validate_parameters(mmu, config);
    if (ret == SUCCESS) {
        ret = mmu_stage2_init(mmu, config);
    }

    return ret;
}
#endif
