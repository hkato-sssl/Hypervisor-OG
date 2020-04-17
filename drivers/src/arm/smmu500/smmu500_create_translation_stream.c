/*
 * arm/smmu500/smmu500_create_translation_stream.c
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

errno_t smmu500_create_translation_stream(struct smmu500 *smmu, uint8_t *id, const struct smmu_translation_stream_configuration *config)
{
    errno_t ret;

    if ((smmu != NULL) && (id != NULL) && (config != NULL)) {
        ret = smmu500_allocate_stream_match_register(smmu, id);
        if (ret == SUCCESS) {
            ret = smmu500_configure_translation_stream(smmu, *id, config);
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

