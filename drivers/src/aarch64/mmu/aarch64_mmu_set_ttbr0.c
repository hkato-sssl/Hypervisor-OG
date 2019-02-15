/* * aarch64/mmu/aarch64_mmu_set_ttbr0.c
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

/* defines */

#define TCR_EL2_RES1    (BIT(31) | BIT(23))
#define TCR_EL3_RES1    (BIT(31) | BIT(23))

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_valid_tt(struct aarch64_mmu_trans_table const *tt)
{
    bool valid;

    if ((tt != NULL) &&
        (tt->addr != NULL) && (((uintptr_t)(tt->addr) & 63) == 0)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}


static void mmu_set_ttbr0(struct aarch64_mmu_trans_table const *tt)
{
    uint64_t d;

    d = ((uint64_t)(tt->asid) << 48) | ((uintptr_t)(tt->addr) & BITS(47, 0));
    aarch64_write_ttbr0(d);
}

static errno_t mmu_set_ttbr0_el1(struct aarch64_mmu_trans_table const *tt)
{
    bool lock;
    uint64_t d;
    uint64_t d0;

    /* TBI0=0, TBI1=0, AS=1, IPS=0b001(36 bits,64GB), TG0=0b10(4KB), EPD0=0 */ 
    d = BIT(36) | (1ULL << 32) | 0x8000;

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint64_t)(tt->tcr.sh) << 12;
    d |= (uint64_t)(tt->tcr.orgn) << 10;
    d |= (uint64_t)(tt->tcr.irgn) << 8;
    d |= (uint64_t)(tt->tcr.sz);

    lock = aarch64_lock();

    d0 = aarch64_read_tcr_el1();
    d |= d0 & BITS(31, 16);
    aarch64_write_tcr_el1(d);

    mmu_set_ttbr0(tt);

    aarch64_unlock(lock);

    return SUCCESS;
}

errno_t aarch64_mmu_set_ttbr0_el1(struct aarch64_mmu_trans_table const *tt)
{
    errno_t ret;

    if (is_valid_tt(tt)) {
        ret = mmu_set_ttbr0_el1(tt);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_set_ttbr0_el2(struct aarch64_mmu_trans_table const *tt)
{
    bool lock;
    uint32_t d;
    uint32_t d0;

    /* TBI=0, PS=0b001(36 bits,64GB), TG0=0b00(4KB) */ 
    d = (1UL << 16) | TCR_EL2_RES1;

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint32_t)(tt->tcr.sh) << 12;
    d |= (uint32_t)(tt->tcr.orgn) << 10;
    d |= (uint32_t)(tt->tcr.irgn) << 8;
    d |= (uint32_t)(tt->tcr.sz);

    lock = aarch64_lock();

    d0 = aarch64_read_tcr_el2();
    d |= d0 & BITS(31, 16);
    aarch64_write_tcr_el2(d);

    mmu_set_ttbr0(tt);

    aarch64_unlock(lock);

    return SUCCESS;
}

errno_t aarch64_mmu_set_ttbr0_el2(struct aarch64_mmu_trans_table const *tt)
{
    errno_t ret;

    if (is_valid_tt(tt)) {
        ret = mmu_set_ttbr0_el2(tt);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t mmu_set_ttbr0_el3(struct aarch64_mmu_trans_table const *tt)
{
    bool lock;
    uint32_t d;
    uint32_t d0;

    /* TBI=0, PS=0b001(36 bits,64GB), TG0=0b00(4KB) */ 
    d = (1UL << 16) | TCR_EL3_RES1;

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint32_t)(tt->tcr.sh) << 12;
    d |= (uint32_t)(tt->tcr.orgn) << 10;
    d |= (uint32_t)(tt->tcr.irgn) << 8;
    d |= (uint32_t)(tt->tcr.sz);

    lock = aarch64_lock();

    d0 = aarch64_read_tcr_el3();
    d |= d0 & BITS(31, 16);
    aarch64_write_tcr_el3(d);

    mmu_set_ttbr0(tt);

    aarch64_unlock(lock);

    return SUCCESS;
}

errno_t aarch64_mmu_set_ttbr0_el3(struct aarch64_mmu_trans_table const *tt)
{
    errno_t ret;

    if (is_valid_tt(tt)) {
        ret = mmu_set_ttbr0_el3(tt);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

