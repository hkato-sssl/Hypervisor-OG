/*
 * aarch64/mmu/aarch64_mmu_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register/tcr.h"
#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint64_t create_tcr_el0(struct aarch64_mmu_configuration const *config)
{
    uint64_t d;

    d = TCR_EL1_AS;
    d |= TCR_EL1_TG0(config->tcr.el0.tg0);
    d |= TCR_EL1_SH0(config->tcr.el0.sh0);
    d |= TCR_EL1_ORGN0(config->tcr.el0.orgn0); 
    d |= TCR_EL1_IRGN0(config->tcr.el0.irgn0); 
    d |= TCR_EL1_T0SZ(config->tcr.el0.t0sz);

    return d;
}

static uint64_t create_tcr_el1(struct aarch64_mmu_configuration const *config)
{
    uint64_t d;

    d = 0;
    if (config->tcr.el1.as != 0) {
        d |= TCR_EL1_AS;
    }
    d |= TCR_EL1_IPS((uint64_t)config->tcr.el1.ips);
    d |= TCR_EL1_TG1(config->tcr.el1.tg1);
    d |= TCR_EL1_SH1(config->tcr.el1.sh1);
    d |= TCR_EL1_ORGN1(config->tcr.el1.orgn1); 
    d |= TCR_EL1_IRGN1(config->tcr.el1.irgn1); 
    if (config->tcr.el1.a1 != 0) {
        d |= TCR_EL1_A1;
    }
    d |= TCR_EL1_T1SZ(config->tcr.el1.t1sz);

    return d;
}

static uint64_t create_tcr_el23(struct aarch64_mmu_configuration const *config)
{
    uint64_t d;

    d = TCR_EL2_RES1;
    d |= TCR_EL2_PS(config->tcr.el23.ps);
    d |= TCR_EL2_TG0(config->tcr.el23.tg0);
    d |= TCR_EL2_SH0(config->tcr.el23.sh0);
    d |= TCR_EL2_ORGN0(config->tcr.el23.orgn0); 
    d |= TCR_EL2_IRGN0(config->tcr.el23.irgn0); 
    d |= TCR_EL2_T0SZ(config->tcr.el23.t0sz);

    return d;
}

static bool is_valid_type(struct aarch64_mmu_configuration const *config)
{
    bool valid;

    if ((config->type == AARCH64_MMU_STAGE2) ||
        (config->type == AARCH64_MMU_EL0) ||
        (config->type == AARCH64_MMU_EL1) ||
        (config->type == AARCH64_MMU_EL2) ||
        (config->type == AARCH64_MMU_EL3)) {
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
        if (is_valid_type(config) && (config->granule == AARCH64_MMU_4KB_GRANULE)) {
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
    if (mmu->type == AARCH64_MMU_STAGE2) {
        mmu->stage2.vmid = config->stage2.vmid;
    } else {
        mmu->stage1.asid = config->stage1.asid;
        mmu->stage1.mair = config->stage1.mair;
        if (mmu->type == AARCH64_MMU_EL0) {
            mmu->stage1.tcr = create_tcr_el0(config);
        } else if (mmu->type == AARCH64_MMU_EL1) {
            mmu->stage1.tcr = create_tcr_el1(config);
        } else {
            mmu->stage1.tcr = create_tcr_el23(config);
        }
    }
    memset(mmu->addr, 0, MMU_BLOCK_SZ);
    aarch64_dcache_clean_range(mmu->addr, MMU_BLOCK_SZ);

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

