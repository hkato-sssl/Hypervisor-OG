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
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

#define TCR_EL23_RES1   (BIT(31) | BIT(23))
#define VTCR_EL2_RES1   BIT(31)

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t validate_parameters(struct aarch64_mmu const *mmu)
{
    errno_t ret;

    if ((mmu != NULL) &&
        (mmu->addr != NULL) && (((uintptr_t)(mmu->addr) & 63) == 0)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static void mmu_set_ttbr0(struct aarch64_mmu const *mmu)
{
    uint64_t d;

    d = ((uint64_t)(mmu->stage1.asid) << 48) | ((uintptr_t)(mmu->addr) & BITS(47, 0));
    aarch64_write_ttbr0_el1(d);
}

static void mmu_set_vttbr(struct aarch64_mmu const *mmu)
{
    uint64_t d;

    d = ((uint64_t)(mmu->stage2.vmid) << 48) | ((uintptr_t)(mmu->addr) & BITS(47, 0));
    aarch64_write_vttbr_el2(d);
}

static errno_t mmu_set_tt_el1(struct aarch64_mmu *mmu)
{
    uint32_t lock;
    uint64_t d;
    uint64_t d0;

    lock = aarch64_lock_interrupts();

    d0 = aarch64_read_tcr_el1();
    /********************************/
    aarch64_write_tcr_el1(d);
    aarch64_write_mair_el1(mmu->stage1.mair);

    mmu_set_ttbr0(mmu);

    mmu->active = true;

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_tt_el23(struct aarch64_mmu *mmu)
{
    uint32_t lock;
    uint32_t d;

    lock = aarch64_lock_interrupts();

    aarch64_write_tcr(mmu->stage1.tcr);
    aarch64_write_mair(mmu->stage1.mair);
    mmu_set_ttbr0(mmu);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_stage2_tt(struct aarch64_mmu *mmu)
{
    uint32_t lock;
    uint32_t d;

    lock = aarch64_lock_interrupts();

    aarch64_write_vtcr_el2(mmu->stage2.vtcr);
    mmu_set_vttbr(mmu);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

errno_t aarch64_mmu_set_tt(struct aarch64_mmu *mmu)
{
    errno_t ret;
    uint64_t el;

    ret = validate_parameters(mmu);
    if (ret == SUCCESS) {
        el = aarch64_read_currentel();
    if ((el == CURRENT_EL1) && (mmu->type == AARCH64_MMU_EL1)) {
            ret = mmu_set_tt_el1(mmu);
        } else if (el == CURRENT_EL2) {
            if (mmu->type == AARCH64_MMU_EL2) {
                ret = mmu_set_tt_el23(mmu);
            } else if (mmu->type == AARCH64_MMU_STAGE2) {
                ret = mmu_set_stage2_tt(mmu);
            } else {
                ret = -EINVAL;
            }
        } else if ((el == CURRENT_EL3) && (mmu->type == AARCH64_MMU_EL3)) {
            if (mmu->stage1.asid == 0) {
                ret = mmu_set_tt_el23(mmu);
            } else {
                ret = -EINVAL;
            }
        } else {
            ret = -EPERM;
        }
    }

    return ret;
}

