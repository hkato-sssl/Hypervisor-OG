/*
 * driver/arm/smmu500.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_SMMU500_H
#define DRIVER_ARM_SMMU500_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

/* types */

struct aarch64_mmu;
struct aatch64_mmu_attr;

struct smmu500 {
    uintptr_t   smmu_base;          /* also used as SMMU_GR0_BASE */
    uintptr_t   smmu_gr1_base;
    uintptr_t   smmu_cb_base;

    uint32_t    nr_pages;
    uint32_t    page_size;          /* 4096 or 65536 */

    uint16_t     nr_context_banks;
    uint16_t     nr_s2_context_banks;
};

struct smmu500_configuration {
    uintptr_t               smmu_base;
    struct aarch64_mmu      *mmu;
    struct aarch64_mmu_attr *mmu_attr;
};

/* variables */

/* functions */

errno_t smmu500_initialize(struct smmu500 *smmu, const struct smmu500_configuration *config);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_SMMU500_H */

