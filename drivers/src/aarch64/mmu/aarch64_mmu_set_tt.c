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

    /* TBI0=0, TBI1=0, AS=1, IPS=0b001(36 bits,64GB), TG0=0b00(4KB), EPD0=0 */ 
    d = BIT(36) | (1ULL << 32);

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint64_t)(mmu->tcr.sh) << 12;
    d |= (uint64_t)(mmu->tcr.orgn) << 10;
    d |= (uint64_t)(mmu->tcr.irgn) << 8;
    d |= (uint64_t)(mmu->tcr.sz);

    lock = aarch64_lock_interrupts();

    d0 = aarch64_read_tcr_el1();
    /* A1=0 */
    d |= d0 & (BITS(31, 23) | BITS(21, 16));
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

    /* TBI=0, PS=0b001(36 bits,64GB), TG0=0b00(4KB) */ 
    d = (1UL << 16) | TCR_EL23_RES1;

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint32_t)(mmu->tcr.sh) << 12;
    d |= (uint32_t)(mmu->tcr.orgn) << 10;
    d |= (uint32_t)(mmu->tcr.irgn) << 8;
    d |= (uint32_t)(mmu->tcr.sz);

    lock = aarch64_lock_interrupts();

    aarch64_write_tcr(d);
    aarch64_write_mair(mmu->stage1.mair);
    mmu_set_ttbr0(mmu);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_stage2_tt(struct aarch64_mmu *mmu)
{
    uint32_t lock;
    uint32_t d;

    /* PS=0b001(36 bits,64GB), TG0=0b00(4KB), SL0=0b10(start at level 0) */ 
    d = VTCR_EL2_RES1 | (1 << 16);

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint32_t)(mmu->tcr.sh) << 12;
    d |= (uint32_t)(mmu->tcr.orgn) << 10;
    d |= (uint32_t)(mmu->tcr.irgn) << 8;
    d |= (uint32_t)(mmu->tcr.sz);

    lock = aarch64_lock_interrupts();

    aarch64_write_vtcr_el2(d);
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
    if ((el == CURRENT_EL1) && (mmu->stage == AARCH64_MMU_STAGE1)) {
            ret = mmu_set_tt_el1(mmu);
        } else if (el == CURRENT_EL2) {
            if (mmu->stage == AARCH64_MMU_STAGE1) {
                ret = mmu_set_tt_el23(mmu);
            } else if (mmu->stage == AARCH64_MMU_STAGE2) {
                ret = mmu_set_stage2_tt(mmu);
            } else {
                ret = -EINVAL;
            }
        } else if ((el == CURRENT_EL3) && (mmu->stage == AARCH64_MMU_STAGE1)) {
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

