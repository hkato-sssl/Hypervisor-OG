/*
 * arm/smmu500/smmu500_configure_context_bank_with_stage2.c
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
#include "driver/arm/device/smmu500/smmu500_s2_cb_actlr.h"
#include "smmu500_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t smmu500_configure_context_bank_with_stage2(struct smmu500 *smmu, uint8_t cb, const struct aarch64_stage2 *stage2)
{
    uint32_t d;
    uint64_t addr;

    addr = (uint64_t)(stage2->base.addr);
    smmu500_cb_write64(smmu, cb, SMMU_S2_CB_TTBR0, addr);

    d = stage2->vtcr_el2 & BITS(18, 0);     /* HD=0, HA=0 */
    smmu500_cb_write32(smmu, cb, SMMU_S2_CB_TCR, d);

    d = SMMU500_S2_CB_ACTLR_CPRE | SMMU500_S2_CB_ACTLR_CMTLB;
    smmu500_cb_write32(smmu, cb, SMMU_S2_CB_ACTLR, d);

    d = SMMU_S2_CB_SCTLR_AFE | SMMU_S2_CB_SCTLR_TRE | SMMU_S2_CB_SCTLR_M;
    smmu500_cb_write32(smmu, cb, SMMU_S2_CB_SCTLR, d);

    return SUCCESS;
}

