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

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t validate_parameters(struct aarch64_mmu_trans_table const *tt)
{
    errno_t ret;

    if ((tt != NULL) &&
        (tt->addr != NULL) && (((uintptr_t)(tt->addr) & 63) == 0)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}


static void mmu_set_ttbr0(struct aarch64_mmu_trans_table const *tt)
{
    uint64_t d;

    d = ((uint64_t)(tt->asid) << 48) | ((uintptr_t)(tt->addr) & BITS(47, 0));
    aarch64_write_ttbr0(d);
}

static errno_t mmu_set_tt_el1(struct aarch64_mmu_trans_table *tt)
{
    uint32_t lock;
    uint64_t d;
    uint64_t d0;

    /* TBI0=0, TBI1=0, AS=1, IPS=0b001(36 bits,64GB), TG0=0b00(4KB), EPD0=0 */ 
    d = BIT(36) | (1ULL << 32);

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint64_t)(tt->tcr.sh) << 12;
    d |= (uint64_t)(tt->tcr.orgn) << 10;
    d |= (uint64_t)(tt->tcr.irgn) << 8;
    d |= (uint64_t)(tt->tcr.sz);

    lock = aarch64_lock_interrupts();

    d0 = aarch64_read_tcr_el1();
    /* A1=0 */
    d |= d0 & (BITS(31, 23) | BITS(21, 16));
    aarch64_write_tcr_el1(d);
    aarch64_write_mair_el1(tt->mair);

    mmu_set_ttbr0(tt);

    tt->active = true;

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t mmu_set_tt_el23(struct aarch64_mmu_trans_table *tt)
{
    uint32_t lock;
    uint32_t d;

    /* TBI=0, PS=0b001(36 bits,64GB), TG0=0b00(4KB) */ 
    d = (1UL << 16) | TCR_EL23_RES1;

    /* set SH0, ORGN0, IRGN0 and T0SZ */
    d |= (uint32_t)(tt->tcr.sh) << 12;
    d |= (uint32_t)(tt->tcr.orgn) << 10;
    d |= (uint32_t)(tt->tcr.irgn) << 8;
    d |= (uint32_t)(tt->tcr.sz);

    lock = aarch64_lock_interrupts();

    aarch64_write_tcr(d);
    aarch64_write_mair(tt->mair);
    mmu_set_ttbr0(tt);

    aarch64_unlock_interrupts(lock);

    return SUCCESS;
}

errno_t aarch64_mmu_set_tt(struct aarch64_mmu_trans_table *tt)
{
    errno_t ret;
    uint64_t el;

    ret = validate_parameters(tt);
    if (ret == SUCCESS) {
        el = aarch64_read_currentel();
        switch (el) {
        case CURRENT_EL1:
            ret = mmu_set_tt_el1(tt);
            break;
        case CURRENT_EL2:
            ret = mmu_set_tt_el23(tt);
            break;
        case CURRENT_EL3:
            if (tt->asid == 0) {
                ret = mmu_set_tt_el23(tt);
            } else {
                ret = -EINVAL;
            }
            break;
        default:
            ret = -EPERM;
            break;
        }
    }

    return ret;
}

