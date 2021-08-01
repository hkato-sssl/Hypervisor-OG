/*
 * arm/smmu500/smmu500_create_context_bank_with_stage2.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/smmu500.h"
#include "smmu500_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t smmu500_create_context_bank_with_stage2(struct smmu500 *smmu, uint8_t *cb, const struct smmu_context_bank_with_stage2_configuration *config)
{
    errno_t ret;

    if ((smmu != NULL) && (cb != NULL) && (config != NULL)) {
        ret = smmu500_allocate_s2_context_bank(smmu, cb);
        if (ret == SUCCESS) {
            ret = smmu500_configure_context_bank_with_stage2(smmu, *cb, config);
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

