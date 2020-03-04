/*
 * arm/smmu500/smmu500_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef ARM_SMMU500_SMMU500_LOCAL_H
#define ARM_SMMU500_SMMU500_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/memio.h"

/* defines */

/* types */

/* variables */

/* functions */

static inline uint32_t smmu500_gr0_read32(struct smmu500 *smmu, uint32_t regs)
{
    uint32_t d;

    d = REG_READ32(smmu->smmu_base, regs);

    return d;
}

static inline void smmu500_gr0_write32(struct smmu500 *smmu, uint32_t regs, uint32_t d)
{
    REG_WRITE32(smmu->smmu_base, regs, d);
}

static inline uint32_t smmu500_gr1_read32(struct smmu500 *smmu, uint32_t regs)
{
    uint32_t d;

    d = REG_READ32(smmu->smmu_gr1_base, regs);

    return d;
}

static inline void smmu500_gr1_write32(struct smmu500 *smmu, uint32_t regs, uint32_t d)
{
    REG_WRITE32(smmu->smmu_gr1_base, regs, d);
}

static inline uint32_t smmu500_cb_read32(struct smmu500 *smmu, uint32_t regs)
{
    uint32_t d;

    d = REG_READ32(smmu->smmu_cb_base, regs);

    return d;
}

static inline void smmu500_cb_write32(struct smmu500 *smmu, uint32_t regs, uint32_t d)
{
    REG_WRITE32(smmu->smmu_cb_base, regs, d);
}

static inline uint32_t smmu500_cb_read64(struct smmu500 *smmu, uint32_t regs)
{
    uint64_t d;

    d = REG_READ64(smmu->smmu_cb_base, regs);

    return d;
}

static inline void smmu500_cb_write64(struct smmu500 *smmu, uint32_t regs, uint64_t d)
{
    REG_WRITE64(smmu->smmu_cb_base, regs, d);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* ARM_SMMU500_SMMU500_LOCAL_H */

