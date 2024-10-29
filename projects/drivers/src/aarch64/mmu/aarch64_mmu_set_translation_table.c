// SPDX-License-Identifier: Apache-2.0
/* * aarch64/mmu/aarch64_mmu_set_translation_table.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64.h"
#include "driver/aarch64/mmu.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/tcr_elx.h"
#include "lib/system/errno.h"
#include "mmu_local.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

#define TCR_EL0_MASK                                                         \
    (TCR_EL1_TBI0 | TCR_EL1_TG0_MASK | TCR_EL1_SH0_MASK | TCR_EL1_ORGN0_MASK \
     | TCR_EL1_IRGN0_MASK | TCR_EL1_EPD0 | TCR_EL1_T0SZ_MASK)

#define TCR_EL1_MASK                                                 \
    (TCR_EL1_TBI1 | TCR_EL1_AS | TCR_EL1_IPS_MASK | TCR_EL1_TG1_MASK \
     | TCR_EL1_SH1_MASK | TCR_EL1_ORGN1_MASK | TCR_EL1_IRGN1_MASK    \
     | TCR_EL1_EPD1 | TCR_EL1_A1 | TCR_EL1_T1SZ_MASK)
/* types */

/* prototypes */

/* variables */

/* functions */

static void mmu_set_ttbr0(struct aarch64_mmu *mmu)
{
    uint64_t d;

    d = ((uint64_t)(mmu->asid) << 48)
        | ((uintptr_t)(mmu->base.addr) & BITS(47, 0));
    aarch64_write_ttbr0(d);
}

static errno_t mmu_set_translation_table_el01(struct aarch64_mmu *mmu)
{
    uint32_t lock;
    uint64_t d;
    uint64_t mask;

    lock = aarch64_lock_interrupts();

    d = aarch64_read_tcr_el1();
    mask = (mmu->base.type == AARCH64_MMU_EL0) ? TCR_EL0_MASK : TCR_EL1_MASK;
    d = (d & ~mask) | mmu->tcr;
    aarch64_write_mair_el1(mmu->mair);

    if (mmu->base.type == AARCH64_MMU_EL1) {
        aarch64_write_tcr_el1(d);
    }

    mmu_set_ttbr0(mmu);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_translation_table_el23(struct aarch64_mmu *mmu)
{
    uint32_t lock;

    lock = aarch64_lock_interrupts();

    if (mmu->base.type == AARCH64_MMU_EL2) {
        aarch64_write_tcr_el2(mmu->tcr);
        aarch64_write_mair_el2(mmu->mair);
    } else {
        aarch64_write_tcr_el3(mmu->tcr);
        aarch64_write_mair_el3(mmu->mair);
    }
    mmu_set_ttbr0(mmu);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_translation_table(struct aarch64_mmu *mmu)
{
    errno_t ret;
    uint64_t el;

    el = aarch64_read_currentel();
    switch (el) {
    case CURRENT_EL1:
        if ((mmu->base.type == AARCH64_MMU_EL0)
            || (mmu->base.type == AARCH64_MMU_EL1)) {
            ret = mmu_set_translation_table_el01(mmu);
        } else {
            ret = -EINVAL;
        }
        break;
    case CURRENT_EL2:
        if (mmu->base.type == AARCH64_MMU_EL2) {
            ret = mmu_set_translation_table_el23(mmu);
        } else {
            ret = -EINVAL;
        }
        break;
    case CURRENT_EL3:
        if (mmu->base.type == AARCH64_MMU_EL3) {
            if (mmu->asid == 0) {
                ret = mmu_set_translation_table_el23(mmu);
            } else {
                ret = -EINVAL;
            }
        } else {
            ret = -EINVAL;
        }
        break;
    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}

static errno_t validate_parameters(struct aarch64_mmu *mmu)
{
    errno_t ret;

    if ((mmu != NULL) && (mmu->base.addr != NULL)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_set_translation_table(struct aarch64_mmu *mmu)
{
    errno_t ret;

    ret = validate_parameters(mmu);
    if (ret == SUCCESS) {
        ret = mmu_set_translation_table(mmu);
    }

    return ret;
}
