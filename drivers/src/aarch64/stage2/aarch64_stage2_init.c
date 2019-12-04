/*
 * aarch64/stage2/aarch64_stage2_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register/vtcr_el2.h"
#include "driver/aarch64/cache.h"
#include "driver/aarch64/stage2.h"
#include "stage2_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static struct aarch64_mmu_ops const mmu_ops = {
    (aarch64_mmu_desc_func_t)aarch64_stage2_table_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_stage2_block_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_stage2_page_descriptor
};

/* functions */

static uint64_t create_vtcr(struct aarch64_stage2_configuration const *config)
{
    uint64_t d;

    d = VTCR_EL2_RES1;
    d |= VTCR_EL2_PS(config->vtcr_el2.ps);
    d |= VTCR_EL2_SH0(config->vtcr_el2.sh0);
    d |= VTCR_EL2_ORGN0(config->vtcr_el2.orgn0);
    d |= VTCR_EL2_IRGN0(config->vtcr_el2.irgn0);
    d |= VTCR_EL2_SL0(2);       /* fixed value */
    d |= VTCR_EL2_T0SZ(config->vtcr_el2.t0sz);

    if (config->base.granule == AARCH64_MMU_64KB_GRANULE) {
        d |= VTCR_EL2_TG0(1);
    } else if (config->base.granule == AARCH64_MMU_16KB_GRANULE) {
        d |= VTCR_EL2_TG0(2);
    }

    return d;
}

static errno_t validate_parameters(struct aarch64_stage2 *mmu, struct aarch64_stage2_configuration const *config)
{
    errno_t ret;

    if ((mmu != NULL) && (config != NULL) && (config->base.pool != NULL) && (config->base.pool->block_sz == MMU_BLOCK_SZ)) {
        if ((config->base.type == AARCH64_MMU_STAGE2) && (config->base.granule == AARCH64_MMU_4KB_GRANULE)) {
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t stage2_init(struct aarch64_stage2 *mmu, struct aarch64_stage2_configuration const *config)
{
    errno_t ret;

    memset(mmu, 0, sizeof(*mmu));

    mmu->base.active = false;
    mmu->base.type = config->base.type;
    mmu->base.granule = config->base.granule;
    mmu->base.ops = &mmu_ops;
    mmu->base.pool = config->base.pool;
    mmu->vmid = config->vmid;
    mmu->vtcr_el2 = create_vtcr(config);

    mmu->base.addr = aarch64_mmu_block_calloc(mmu->base.pool, MMU_BLOCK_SZ);
    if (mmu->base.addr != NULL) {
        ret = SUCCESS;
    } else {
        ret = -ENOMEM;
    }

    return ret;
}

errno_t aarch64_stage2_init(struct aarch64_stage2 *mmu, struct aarch64_stage2_configuration const *config)
{
    errno_t ret;

    ret = validate_parameters(mmu, config);
    if (ret == SUCCESS) {
        ret = stage2_init(mmu, config);
    }

    return ret;
}

