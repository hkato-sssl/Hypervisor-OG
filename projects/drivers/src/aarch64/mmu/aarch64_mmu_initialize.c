// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/mmu/aarch64_mmu_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/mmu.h"
#include "driver/aarch64/system_register/tcr_elx.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "mmu_local.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

static const struct aarch64_mmu_ops mmu_ops = {
    (aarch64_mmu_desc_func_t)aarch64_mmu_table_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_mmu_block_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_mmu_page_descriptor};

/* functions */

static uint64_t create_tcr_el0(const struct aarch64_mmu_configuration *config)
{
    uint64_t d;

    d = TCR_EL1_AS;
    d |= TCR_EL1_SH0(config->tcr.el0.sh0);
    d |= TCR_EL1_ORGN0(config->tcr.el0.orgn0);
    d |= TCR_EL1_IRGN0(config->tcr.el0.irgn0);
    d |= TCR_EL1_T0SZ(config->tcr.el0.t0sz);

    if (config->base.granule == AARCH64_MMU_4KB_GRANULE) {
        d |= TCR_EL1_TG1(1);
    } else if (config->base.granule == AARCH64_MMU_16KB_GRANULE) {
        d |= TCR_EL1_TG1(2);
    }

    return d;
}

static uint64_t create_tcr_el1(const struct aarch64_mmu_configuration *config)
{
    uint64_t d;

    d = 0;
    if (config->tcr.el1.as != 0) {
        d |= TCR_EL1_AS;
    }
    d |= TCR_EL1_IPS((uint64_t)config->tcr.el1.ips);
    d |= TCR_EL1_SH1(config->tcr.el1.sh1);
    d |= TCR_EL1_ORGN1(config->tcr.el1.orgn1);
    d |= TCR_EL1_IRGN1(config->tcr.el1.irgn1);
    if (config->tcr.el1.a1 != 0) {
        d |= TCR_EL1_A1;
    }
    d |= TCR_EL1_T1SZ(config->tcr.el1.t1sz);

    if (config->base.granule == AARCH64_MMU_16KB_GRANULE) {
        d |= TCR_EL1_TG1(1);
    } else if (config->base.granule == AARCH64_MMU_4KB_GRANULE) {
        d |= TCR_EL1_TG1(2);
    } else {
        d |= TCR_EL1_TG1(3); /* 64KB granule */
    }

    return d;
}

static uint64_t create_tcr_el23(const struct aarch64_mmu_configuration *config)
{
    uint64_t d;

    d = TCR_EL2_RES1;
    d |= TCR_EL2_PS(config->tcr.el23.ps);
    d |= TCR_EL2_SH0(config->tcr.el23.sh0);
    d |= TCR_EL2_ORGN0(config->tcr.el23.orgn0);
    d |= TCR_EL2_IRGN0(config->tcr.el23.irgn0);
    d |= TCR_EL2_T0SZ(config->tcr.el23.t0sz);

    /* TG0=0: 4KB granule */
    if (config->base.granule == AARCH64_MMU_64KB_GRANULE) {
        d |= TCR_EL2_TG0(1);
    } else if (config->base.granule == AARCH64_MMU_16KB_GRANULE) {
        d |= TCR_EL2_TG0(2);
    }

    return d;
}

static bool is_valid_type(const struct aarch64_mmu_configuration *config)
{
    bool ret;

    if ((config->base.type == AARCH64_MMU_EL0)
        || (config->base.type == AARCH64_MMU_EL1)
        || (config->base.type == AARCH64_MMU_EL2)
        || (config->base.type == AARCH64_MMU_EL3)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static errno_t
validate_parameters(struct aarch64_mmu *mmu,
                    const struct aarch64_mmu_configuration *config)
{
    errno_t ret;

    if (mmu == NULL) {
        ret = -EFAULT;
    } else if (config == NULL) {
        ret = -EFAULT;
    } else if (config->base.pool == NULL) {
        ret = -EFAULT;
    } else if (config->base.pool->block_sz != MMU_BLOCK_SZ) {
        ret = -EINVAL;
    } else if (! is_valid_type(config)) {
        ret = -EINVAL;
    } else if (config->base.granule != AARCH64_MMU_4KB_GRANULE) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t mmu_initialize(struct aarch64_mmu *mmu,
                              const struct aarch64_mmu_configuration *config)
{
    errno_t ret;

    memset(mmu, 0, sizeof(*mmu));

    spin_lock_init(&(mmu->base.lock));
    mmu->base.type = config->base.type;
    mmu->base.granule = config->base.granule;
    mmu->base.pool = config->base.pool;
    mmu->base.ops = &mmu_ops;
    mmu->base.start_level = 0;
    mmu->asid = config->asid;
    mmu->mair = config->mair;
    if (mmu->base.type == AARCH64_MMU_EL0) {
        mmu->tcr = create_tcr_el0(config);
    } else if (mmu->base.type == AARCH64_MMU_EL1) {
        mmu->tcr = create_tcr_el1(config);
    } else {
        mmu->tcr = create_tcr_el23(config);
    }

    mmu->base.addr = aarch64_mmu_block_calloc(mmu->base.pool, MMU_BLOCK_SZ);
    if (mmu->base.addr != NULL) {
        ret = SUCCESS;
    } else {
        ret = -ENOMEM;
    }

    return ret;
}

errno_t aarch64_mmu_initialize(struct aarch64_mmu *mmu,
                               const struct aarch64_mmu_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(mmu, config);
    if (ret == SUCCESS) {
        ret = mmu_initialize(mmu, config);
    }

    return ret;
}
