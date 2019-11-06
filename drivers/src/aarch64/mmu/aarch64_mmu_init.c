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

static bool is_valid_stage(struct aarch64_mmu_trans_table_configuration const *config)
{
    bool valid;

    if ((config->stage == AARCH64_MMU_STAGE1) || (config->stage == AARCH64_MMU_STAGE2)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

static errno_t validate_parameters(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
    errno_t ret;

    if ((tt != NULL) && (config != NULL) && (config->pool.block_sz == MMU_BLOCK_SZ)) {
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

static errno_t init_trans_table(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
    tt->active = false;
    tt->stage = config->stage;
    tt->granule = config->granule;
    if (tt->stage == AARCH64_MMU_STAGE1) {
        tt->stage1.asid = config->stage1.asid;
        tt->stage1.mair = config->stage1.mair;
    } else {
        tt->stage2.vmid = config->stage2.vmid;
    }
    memset(tt->addr, 0, MMU_BLOCK_SZ);

    tt->tcr = config->tcr;

    return SUCCESS;
}

static errno_t mmu_init(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
    errno_t ret;

    memset(tt, 0, sizeof(*tt));

    ret = aarch64_mmu_block_pool_init(&(tt->pool), &(config->pool));
    if (ret == SUCCESS) {
        tt->addr = aarch64_mmu_block_calloc(&(tt->pool), MMU_BLOCK_SZ);
        if (tt->addr != NULL) {
            ret = init_trans_table(tt, config);
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t aarch64_mmu_init(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
    errno_t ret;

    ret = validate_parameters(tt, config);
    if (ret == SUCCESS) {
        ret = mmu_init(tt, config);
    }

    return ret;
}

