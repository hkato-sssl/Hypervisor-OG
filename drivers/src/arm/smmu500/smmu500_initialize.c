/*
 * arm/smmu500/smmu500_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "driver/arm/smmu500.h"
#include "driver/arm/device/smmu500.h"
#include "driver/arm/device/smmuv2/smmu_idr1.h"
#include "smmu500_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t map(const struct smmu500 *smmu, const struct smmu500_configuration *config, uintptr_t offset, uint32_t nr_pages)
{
    errno_t ret;
    void *addr;
    size_t size;

    addr = (void *)(smmu->smmu_base + offset);
    size = nr_pages * smmu->page_size;
    ret = aarch64_mmu_map(config->mmu, addr, addr, size, config->mmu_attr);

    return ret;
}

static errno_t map_register_region(const struct smmu500 *smmu, const struct smmu500_configuration *config)
{
    errno_t ret;
    uintptr_t offset;

    /* map SMMU Global Register Space 1 */
    offset = smmu->page_size;
    ret = map(smmu, config, offset, 1);

    if (ret == SUCCESS) {
        /* map Translation context bank address space */
        offset = smmu->nr_pages * smmu->page_size;
        ret = map(smmu, config, offset, smmu->nr_pages);
    }

    return ret;
}

static void probe_device(struct smmu500 *smmu)
{
    uint32_t d;
    uint32_t idr1;

    idr1 = smmu500_gr0_read32(smmu, SMMU_IDR1);

    if ((idr1 & SMMU_IDR1_PAGESIZE) != 0) {
        smmu->page_size = 64 * 1024;
    } else {
        smmu->page_size = 4 * 1024;
    }

    smmu->smmu_gr1_base = smmu->smmu_base + smmu->page_size;
    smmu->smmu_cb_base = smmu->smmu_base + (smmu->page_size * smmu->nr_pages);

    d = EXTRACT_SMMU_IDR1_NUMPAGENDXB(idr1);
    smmu->nr_pages = 1 << (d + 1);

    smmu->nr_context_banks = EXTRACT_SMMU_IDR1_NUMCB(idr1);
    smmu->nr_s2_context_banks = EXTRACT_SMMU_IDR1_NUMS2CB(idr1);
}

static errno_t initialize(struct smmu500 *smmu, const struct smmu500_configuration *config)
{
    errno_t ret;

    smmu->smmu_base = config->smmu_base;
    probe_device(smmu);
    ret = map_register_region(smmu, config);

    return ret;
}

static errno_t validate_parameters(const struct smmu500 *smmu, const struct smmu500_configuration *config)
{
    errno_t ret;

    if ((smmu != NULL) && (config != NULL) && (config->smmu_base != 0) && (config->mmu != NULL) && (config->mmu_attr != NULL)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t smmu500_initialize(struct smmu500 *smmu, const struct smmu500_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(smmu, config);
    if (ret == SUCCESS) {
        ret = initialize(smmu, config);
    }

    return ret;
}

