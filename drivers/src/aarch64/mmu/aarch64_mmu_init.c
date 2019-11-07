/*
 * aarch64/mmu/aarch64_mmu_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_valid_stage(struct aarch64_mmu_configuration const *config)
{
    bool valid;

    if ((config->stage == AARCH64_MMU_STAGE1) || (config->stage == AARCH64_MMU_STAGE2)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

static errno_t validate_parameters(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config)
{
    errno_t ret;

    if ((mmu != NULL) && (config != NULL) && (config->pool.block_sz == MMU_BLOCK_SZ)) {
        if (is_valid_stage(config) && (config->granule == AARCH64_MMU_4KB_GRANULE)) {
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
    mmu->stage = config->stage;
    mmu->granule = config->granule;
    if (mmu->stage == AARCH64_MMU_STAGE1) {
        mmu->stage1.asid = config->stage1.asid;
        mmu->stage1.mair = config->stage1.mair;
    } else {
        mmu->stage2.vmid = config->stage2.vmid;
    }
    memset(mmu->addr, 0, MMU_BLOCK_SZ);

    mmu->tcr = config->tcr;

    return SUCCESS;
}

static errno_t mmu_init(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config)
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

errno_t aarch64_mmu_init(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config)
{
    errno_t ret;

    ret = validate_parameters(mmu, config);
    if (ret == SUCCESS) {
        ret = mmu_init(mmu, config);
    }

    return ret;
}

