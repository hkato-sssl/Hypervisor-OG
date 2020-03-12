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

static inline uint32_t smmu500_gr0_read32(const struct smmu500 *smmu, uint32_t regs)
{
    uint32_t d;

    d = REG_READ32(smmu->smmu_base, regs);

    return d;
}

static inline void smmu500_gr0_write32(const struct smmu500 *smmu, uint32_t regs, uint32_t d)
{
    REG_WRITE32(smmu->smmu_base, regs, d);
}

static inline uint32_t smmu500_gr1_read32(const struct smmu500 *smmu, uint32_t regs)
{
    uint32_t d;

    d = REG_READ32(smmu->smmu_gr1_base, regs);

    return d;
}

static inline void smmu500_gr1_write32(const struct smmu500 *smmu, uint32_t regs, uint32_t d)
{
    REG_WRITE32(smmu->smmu_gr1_base, regs, d);
}

static inline uintptr_t smmu500_cb_base(const struct smmu500 *smmu, uint8_t cb)
{
    return (smmu->smmu_cb_base + (cb * smmu->page_size));
}

static inline uint32_t smmu500_cb_read32(const struct smmu500 *smmu, uint8_t cb, uint32_t regs)
{
    uint32_t d;
    uintptr_t base;

    base = smmu500_cb_base(smmu, cb);
    d = REG_READ32(base, regs);

    return d;
}

static inline void smmu500_cb_write32(const struct smmu500 *smmu, uint8_t cb, uint32_t regs, uint32_t d)
{
    uintptr_t base;

    base = smmu500_cb_base(smmu, cb);
    REG_WRITE32(base, regs, d);
}

static inline uint32_t smmu500_cb_read64(const struct smmu500 *smmu, uint8_t cb, uint32_t regs)
{
    uint64_t d;
    uintptr_t base;

    base = smmu500_cb_base(smmu, cb);
    d = REG_READ64(base, regs);

    return d;
}

static inline void smmu500_cb_write64(const struct smmu500 *smmu, uint8_t cb, uint32_t regs, uint64_t d)
{
    uintptr_t base;

    base = smmu500_cb_base(smmu, cb);
    REG_WRITE64(base, regs, d);
}

errno_t smmu500_allocate(uint8_t *id, struct smmu500 *smmu, void *map, size_t map_size, uint32_t nr_bits);
errno_t smmu500_allocate_stream_map(uint8_t *id, struct smmu500 *smmu);
errno_t smmu500_allocate_context_bank(uint8_t *id, struct smmu500 *smmu);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* ARM_SMMU500_SMMU500_LOCAL_H */

