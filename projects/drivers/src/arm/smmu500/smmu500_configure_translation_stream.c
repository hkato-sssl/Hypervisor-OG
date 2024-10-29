// SPDX-License-Identifier: Apache-2.0
/*
 * arm/smmu500/smmu500_configure_translation_stream.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/smmuv2.h"
#include "driver/arm/device/smmuv2/smmu_s2cr.h"
#include "driver/arm/smmu500.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "smmu500_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t smmu500_configure_translation_stream(
    struct smmu500 *smmu, uint8_t id,
    const struct smmu_translation_stream_configuration *config)
{
    uint32_t d;

    /* configure SMMU_SMRn */

    d = ((uint32_t)(config->stream.mask) << 16) | config->stream.id;
    d &= BITS(30, 0); /* clear MSB */

    smmu500_gr0_write32(smmu, SMMU_SMR(id), d);

    /* configure SMMU_S2CRn */

    d = SMMU_S2CR_TRANSIENTCFG(config->transientcfg)
        | SMMU_S2CR_INSTCFG(config->instcfg)
        | SMMU_S2CR_PRIVCFG(config->privcfg) | SMMU_S2CR_WACFG(config->wacfg)
        | SMMU_S2CR_RACFG(config->racfg) | SMMU_S2CR_NSCFG(config->nscfg)
        | SMMU_S2CR_SHCFG(config->shcfg);

    if (config->flag.exidvalid) {
        d |= SMMU_S2CR_EXIDVALID;
    }

    if (config->mtcfg != 0) {
        d |= SMMU_S2CR_MTCFG | SMMU_S2CR_MEMATTR(config->memattr);
    }

    d |= SMMU_S2CR_CBNDX(config->cbndx);

    smmu500_gr0_write32(smmu, SMMU_S2CR(id), d);

    return SUCCESS;
}
