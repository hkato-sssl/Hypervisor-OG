/*
 * arm/smmu500/smmu500_attach_stage2.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/stage2.h"
#include "driver/arm/smmu500.h"
#include "driver/arm/device/smmuv2.h"
#include "driver/arm/device/smmuv2/smmu_s2_cb.h"
#include "driver/arm/device/smmuv2/smmu_s2cr_translation.h"
#include "driver/arm/device/smmu500/smmu500_s2_cb_actlr.h"
#include "smmu500_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t assign_stream_to_context(struct smmu500 *smmu, const struct smmu500_attach_stage2_configuration *config, uint8_t id, uint8_t s2)
{
    uint32_t d;

    /* S2CRn */

    d = SMMU_S2CR_TRANSLATION_TYPE(0);
    if (config->translation.flag.transient) {
        d |= SMMU_S2CR_TRANSLATION_TRANSIENTCFG(3); /* Transient */
    }
    if (config->translation.flag.instruction) {
        d |= SMMU_S2CR_TRANSLATION_INSTCFG(3);  /* Instruction */
    } else {
        d |= SMMU_S2CR_TRANSLATION_INSTCFG(2);  /* Data */
    }
    if (config->translation.flag.unprivileged) {
        d |= SMMU_S2CR_TRANSLATION_PRIVCFG(2);  /* Unprivileged */
    } else {
        d |= SMMU_S2CR_TRANSLATION_PRIVCFG(3);  /* Privileged */
    }
    if (config->translation.flag.write_allocate) {
        d |= SMMU_S2CR_TRANSLATION_WACFG(2);    /* Write-Allocate */
    } else {
        d |= SMMU_S2CR_TRANSLATION_WACFG(3);    /* No Write-Allocate */
    }
    if (config->translation.flag.read_allocate) {
        d |= SMMU_S2CR_TRANSLATION_RACFG(2);    /* Read-Allocate */
    } else {
        d |= SMMU_S2CR_TRANSLATION_RACFG(3);    /* No Read-Allocate */
    }
    if (config->translation.flag.memory_attribute) {
        d |= SMMU_S2CR_TRANSLATION_MEMATTR(config->translation.memory_attribute)
            | SMMU_S2CR_TRANSLATION_MTCFG;
    }
    if (config->translation.flag.extended_id) {
        d |= SMMU_S2CR_TRANSLATION_EXIDVALID;
    }
    if (config->translation.flag.shareability) {
        d |= SMMU_S2CR_TRANSLATION_SHCFG(config->translation.shareability);
    }
    d |= SMMU_S2CR_TRANSLATION_CBNDX(s2);
    smmu500_gr0_write32(smmu, SMMU_S2CR(id), d);

    /* SMRn */

    d = ((config->stream.mask & BITS(14, 0)) << 16)
        | (config->stream.id & BITS(14, 0));
    smmu500_gr0_write32(smmu, SMMU_SMR(id), d);

    return SUCCESS;
}

static errno_t configure_s2_context_bank(struct smmu500 *smmu, const struct smmu500_attach_stage2_configuration *config, uint8_t id, uint8_t s2)
{
    errno_t ret;
    uint32_t d;
    uint64_t addr;

    addr = (uint64_t)(config->stage2->base.addr);
    smmu500_cb_write64(smmu, s2, SMMU_S2_CB_TTBR0, addr);

    d = config->stage2->vtcr_el2 & BITS(18, 0);        /* HD=0, HA=0 */
    smmu500_cb_write32(smmu, s2, SMMU_S2_CB_TCR, d);

    d = SMMU500_S2_CB_ACTLR_CPRE | SMMU500_S2_CB_ACTLR_CMTLB;
    smmu500_cb_write32(smmu, s2, SMMU_S2_CB_ACTLR, d);

    d = SMMU_S2_CB_SCTLR_AFE | SMMU_S2_CB_SCTLR_TRE | SMMU_S2_CB_SCTLR_M;
    smmu500_cb_write32(smmu, s2, SMMU_S2_CB_SCTLR, d);

    ret = assign_stream_to_context(smmu, config, id, s2);

    return ret;
}

static errno_t attach_stage2(uint8_t *id, struct smmu500 *smmu, const struct smmu500_attach_stage2_configuration *config)
{
    errno_t ret;
    uint8_t s2;

    ret = smmu500_allocate_context_bank(&s2, smmu);
    if (ret == SUCCESS) {
        ret = smmu500_allocate_stream_match_register(id, smmu);
        if (ret == SUCCESS) {
            ret = configure_s2_context_bank(smmu, config, *id, s2);
        } else {
            smmu500_free_context_bank(smmu, s2);
        }
    }

    return ret;
}

errno_t smmu500_attach_stage2(uint8_t *id, struct smmu500 *smmu, const struct smmu500_attach_stage2_configuration *config)
{
    errno_t ret;

    if ((id != NULL) && (smmu != NULL) && (config != NULL)) {
        smmu500_lock(smmu);
        ret = attach_stage2(id, smmu, config);
        smmu500_unlock(smmu);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

