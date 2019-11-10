/* * aarch64/mmu/aarch64_mmu_set_tt.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/tcr.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

#define TCR_EL0_MASK    (TCR_EL1_TBI0 |\
                         TCR_EL1_TG0_MASK |\
                         TCR_EL1_SH0_MASK |\
                         TCR_EL1_ORGN0_MASK |\
                         TCR_EL1_IRGN0_MASK |\
                         TCR_EL1_EPD0 |\
                         TCR_EL1_T0SZ_MASK)

#define TCR_EL1_MASK    (TCR_EL1_TBI1 |\
                         TCR_EL1_AS |\
                         TCR_EL1_IPS_MASK |\
                         TCR_EL1_TG1_MASK |\
                         TCR_EL1_SH1_MASK |\
                         TCR_EL1_ORGN1_MASK |\
                         TCR_EL1_IRGN1_MASK |\
                         TCR_EL1_EPD1 |\
                         TCR_EL1_A1 |\
                         TCR_EL1_T1SZ_MASK)
/* types */

/* prototypes */

/* variables */

/* functions */

static void mmu_set_ttbr0(struct aarch64_mmu  *mmu)
{
    uint64_t d;

    d = ((uint64_t)(mmu->stage1.asid) << 48) | ((uintptr_t)(mmu->addr) & BITS(47, 0));
    aarch64_write_ttbr0_el1(d);
}

static void mmu_set_vttbr(struct aarch64_mmu  *mmu)
{
    uint64_t d;

    d = ((uint64_t)(mmu->stage2.vmid) << 48) | ((uintptr_t)(mmu->addr) & BITS(47, 0));
    aarch64_write_vttbr_el2(d);
}

static errno_t mmu_set_tt_el01(struct aarch64_mmu  *mmu)
{
    uint32_t lock;
    uint64_t d;
    uint64_t mask;

    lock = aarch64_lock_interrupts();

    d = aarch64_read_tcr_el1();
    mask = (mmu->type == AARCH64_MMU_EL0) ? TCR_EL0_MASK : TCR_EL1_MASK;
    d = (d & ~mask) | mmu->stage1.tcr;
    aarch64_write_mair_el1(mmu->stage1.mair);

    if (mmu->type == AARCH64_MMU_EL1) {
        aarch64_write_tcr_el1(d);
    }

    mmu_set_ttbr0(mmu);

    mmu->active = true;

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_tt_el23(struct aarch64_mmu  *mmu)
{
    uint32_t lock;

    lock = aarch64_lock_interrupts();

    if (mmu->type == AARCH64_MMU_EL2) {
        aarch64_write_tcr_el2(mmu->stage1.tcr);
        aarch64_write_mair_el2(mmu->stage1.mair);
    } else {
        aarch64_write_tcr_el3(mmu->stage1.tcr);
        aarch64_write_mair_el3(mmu->stage1.mair);
    }
    mmu_set_ttbr0(mmu);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_stage2_tt(struct aarch64_mmu  *mmu)
{
    uint32_t lock;

    lock = aarch64_lock_interrupts();

    aarch64_write_vtcr_el2(mmu->stage2.vtcr);
    mmu_set_vttbr(mmu);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_tt(struct aarch64_mmu  *mmu)
{
    errno_t ret;
    uint64_t el;

    el = aarch64_read_currentel();
    switch (el) {
    case CURRENT_EL1:
        if ((mmu->type == AARCH64_MMU_EL0) || (mmu->type == AARCH64_MMU_EL1)) {
            ret = mmu_set_tt_el01(mmu);
        } else {
            ret = -EINVAL;
        }
        break;
    case CURRENT_EL2:
        if (mmu->type == AARCH64_MMU_EL2) {
            ret = mmu_set_tt_el23(mmu);
        } else if (mmu->type == AARCH64_MMU_STAGE2) {
            ret = mmu_set_stage2_tt(mmu);
        } else {
            ret = -EINVAL;
        }
        break;
    case CURRENT_EL3:
        if (mmu->type == AARCH64_MMU_EL3) {
            if (mmu->stage1.asid == 0) {
                ret = mmu_set_tt_el23(mmu);
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

static errno_t validate_parameters(struct aarch64_mmu  *mmu)
{
    errno_t ret;

    if ((mmu != NULL) && (! mmu->active) && (mmu->addr != NULL)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t aarch64_mmu_set_tt(struct aarch64_mmu  *mmu)
{
    errno_t ret;

    ret = validate_parameters(mmu);
    if (ret == SUCCESS) {
        ret = mmu_set_tt(mmu);
    }
    
    return ret;
}

