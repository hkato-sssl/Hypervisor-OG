/*
 * arm/smmu500/smmu500_disable.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/smmu500.h"
#include "driver/arm/device/smmuv2.h"
#include "smmu500_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t disable(struct smmu500 *smmu, uint8_t id)
{
    errno_t ret;
    uint32_t d;

    smmu500_lock(smmu);

    d = smmu500_gr0_read32(smmu, SMMU_SMR(id));
    if ((d & BIT(31)) != 0) {
        d ^= BIT(31);
        smmu500_gr0_write32(smmu, SMMU_SMR(id), d);
        ret = SUCCESS;
    } else {
        ret = -ENODEV;
    }

    smmu500_unlock(smmu);

    return ret;
}

errno_t smmu500_disable(struct smmu500 *smmu, uint8_t id)
{
    errno_t ret;

    if ((smmu != NULL) && (id < smmu->nr_stream_matches)) {
        ret = disable(smmu, id);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
