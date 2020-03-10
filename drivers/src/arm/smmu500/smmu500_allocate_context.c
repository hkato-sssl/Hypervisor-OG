/*
 * arm/smmu500/smmu500_allocate_context.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/bitmap.h"
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "driver/arm/smmu500.h"
#include "smmu500_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t smmu500_allocate_context(uint8_t *id, struct smmu500 *smmu)
{
    errno_t ret;

    ret = smmu500_allocate(id, smmu, smmu->allocate.contexts, sizeof(smmu->allocate.contexts), smmu->nr_contexts);

    return ret;
}

