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

static int alloc(uint8_t *id, struct smmu500 *smmu)
{
    errno_t ret;
    uint32_t no;

    ret = bitmap_search0(&no, smmu->allocate.contexts, sizeof(smmu->allocate.contexts));
    if (ret == SUCCESS) {
        *id = (uint8_t)no;
        ret = bitmap_set(smmu->allocate.contexts, sizeof(smmu->allocate.contexts), no);
    }

    return ret;
}

errno_t smmu500_allocate_context(uint8_t *id, struct smmu500 *smmu)
{
    errno_t ret;

    spin_lock(&(smmu->lock));
    ret = alloc(id, smmu);
    spin_unlock(&(smmu->lock));

    return ret;
}

