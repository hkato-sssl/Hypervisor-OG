/*
 * arm/smmu500/smmu500_enable.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/smmuv2.h"
#include "driver/arm/smmu500.h"
#include "lib/system/errno.h"
#include "smmu500_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t enable(struct smmu500 *smmu, uint8_t id)
{
    errno_t ret;
    uint32_t d;

    smmu500_lock(smmu);

    d = smmu500_gr0_read32(smmu, SMMU_SMR(id));
    if ((d & BIT(31)) == 0) {
        d |= BIT(31);
        smmu500_gr0_write32_sync(smmu, SMMU_SMR(id), d);
        ret = SUCCESS;
    } else {
        ret = -EBUSY;
    }

    smmu500_unlock(smmu);

    return ret;
}

errno_t smmu500_enable(struct smmu500 *smmu, uint8_t id)
{
    errno_t ret;

    if ((smmu != NULL) && (id < smmu->nr_stream_matches)) {
        ret = enable(smmu, id);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
