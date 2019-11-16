/*
 * driver/aarch64/mmu/aarch64_mmu_stage2_enable.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void set_vttbr_el2(struct aarch64_mmu const *mmu)
{
    uint64_t d;

    d = (uint64_t)(mmu->addr);
    d |= (uint64_t)(mmu->stage2.vmid) << 48;
    aarch64_write_vttbr_el2(d);
}

static void set_vtcr_el2(struct aarch64_mmu const *mmu)
{
}

static errno_t mmu_stage2_enable(struct aarch64_mmu *mmu)
{
    set_vttbr_el2(mmu);
    set_vtcr_el2(mmu);

    return SUCCESS;
}

errno_t aarch64_mmu_stage2_enable(struct aarch64_mmu *mmu)
{
    errno_t ret;

    if ((mmu != NULL) && (mmu->active == false)) {
        ret = mmu_stage2_enable(mmu);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
