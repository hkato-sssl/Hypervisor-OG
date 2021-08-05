/*
 * arm/smmu500/smmu500_configure_context_bank_with_stage2.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/stage2.h"
#include "driver/arm/device/smmu500/smmu500_s2_cb_actlr.h"
#include "driver/arm/device/smmuv2.h"
#include "driver/arm/device/smmuv2/smmu_cba2r.h"
#include "driver/arm/device/smmuv2/smmu_cbar.h"
#include "driver/arm/device/smmuv2/smmu_s2_cb.h"
#include "driver/arm/smmu500.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "smmu500_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure_context_bank_with_stage2(
    struct smmu500 *smmu, uint8_t cb,
    const struct smmu_context_bank_with_stage2_configuration *config)
{
    uint32_t d;
    uint64_t addr;

    addr = (uint64_t)(config->stage2->base.addr);
    smmu500_cb_write64(smmu, cb, SMMU_S2_CB_TTBR0, addr);

    d = config->stage2->vtcr_el2 & BITS(18, 0); /* HD=0, HA=0 */
    smmu500_cb_write32(smmu, cb, SMMU_S2_CB_TCR, d);

    d = SMMU500_S2_CB_ACTLR_CPRE | SMMU500_S2_CB_ACTLR_CMTLB;
    smmu500_cb_write32(smmu, cb, SMMU_S2_CB_ACTLR, d);

    d = SMMU_S2_CB_SCTLR_AFE | SMMU_S2_CB_SCTLR_TRE | SMMU_S2_CB_SCTLR_M;
    if (config->flag.interrupt != 0) {
        d |= SMMU_S2_CB_SCTLR_CFIE;
    }
    if (config->flag.fault != 0) {
        d |= SMMU_S2_CB_SCTLR_CFRE;
    }
    smmu500_cb_write32(smmu, cb, SMMU_S2_CB_SCTLR, d);

    /* SMMU_CBARn */

    if (smmu->nr_context_fault_interrupts > 1) {
        d = SMMU_CBAR_S2_IRPTNDX(config->interrupt_index);
    } else {
        /* SMMUv2:
         * When each context bank has a dedicated interrupt pin,
         * there is no requirement for software to specify the interrupt
         * number.
         */
        d = 0;
    }

    if (smmu->vmid_size == 8) {
        d |= SMMU_CBAR_S2_VMID(config->vmid);
    }

    smmu500_gr1_write32(smmu, SMMU_CBAR(cb), d);

    /* SMMU_CBA2Rn */

    d = SMMU_CBA2R_VA64;
    if (smmu->vmid_size == 16) {
        d |= SMMU_CBA2R_VMID16(config->vmid);
    }

    smmu500_gr1_write32(smmu, SMMU_CBA2R(cb), d);

    return SUCCESS;
}

errno_t smmu500_configure_context_bank_with_stage2(
    struct smmu500 *smmu, uint8_t cb,
    const struct smmu_context_bank_with_stage2_configuration *config)
{
    errno_t ret;

    if (config->stage2 != NULL) {
        if (config->interrupt_index < smmu->nr_context_fault_interrupts) {
            ret = configure_context_bank_with_stage2(smmu, cb, config);
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}
