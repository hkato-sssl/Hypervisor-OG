/*
 * arm/smmu500/smmu500_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/mmu.h"
#include "driver/arm/device/smmu500.h"
#include "driver/arm/device/smmuv2/smmu_cr0.h"
#include "driver/arm/device/smmuv2/smmu_idr0.h"
#include "driver/arm/device/smmuv2/smmu_idr1.h"
#include "driver/arm/device/smmuv2/smmu_idr2.h"
#include "driver/arm/smmu500.h"
#include "lib/system/errno.h"
#include "smmu500_local.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t initialize_device(struct smmu500 *smmu,
                                 const struct smmu500_configuration *config)
{
    uint32_t i;
    uint32_t d;

    /* disable SMMU */
    smmu500_gr0_write32_sync(smmu, SMMU_sCR0, SMMU_CR0_CLIENTPD);

    smmu500_gr0_write32(smmu, SMMU_sGFAR, 0);
    smmu500_gr0_write32(smmu, SMMU_sGFSR, ~(uint32_t)0);
    smmu500_gr0_write32(smmu, SMMU_sGFSRRESTORE, 0);
    smmu500_gr0_write32(smmu, SMMU_sGFSYNR0, 0);
    smmu500_gr0_write32(smmu, SMMU_sGFSYNR1, 0);
    smmu500_gr0_write32(smmu, SMMU_sGFSYNR2, 0);

    /* clear SMMU_SMRn */
    for (i = 0; i < smmu->nr_stream_matches; ++i) {
        smmu500_gr0_write32(smmu, SMMU_SMR(i), 0);
        smmu500_gr0_write32(smmu, SMMU_S2CR(i), 0);
        smmu500_gr1_write32(smmu, SMMU_CBAR(i), 0);
        smmu500_gr1_write32(smmu, SMMU_CBA2R(i), 0);
        smmu500_gr1_write32(smmu, SMMU_CBFRSYNRA(i), 0);
    }

    /* enable SMMU */
    d = 0;
    if (config->flag.interrupt) {
        d |= SMMU_CR0_SMCFCFG | SMMU_CR0_USFCFG | SMMU_CR0_GCFGFIE
             | SMMU_CR0_GFIE;
    }
    if (config->flag.fault) {
        d |= SMMU_CR0_SMCFCFG | SMMU_CR0_USFCFG | SMMU_CR0_GCFGFRE
             | SMMU_CR0_GFRE;
    }
    smmu500_gr0_write32_sync(smmu, SMMU_sCR0, d);

    return SUCCESS;
}

static errno_t map(const struct smmu500 *smmu,
                   const struct smmu500_configuration *config, uintptr_t offset,
                   uint32_t nr_pages)
{
    errno_t ret;
    void *addr;
    size_t size;

    addr = (void *)(smmu->smmu_base + offset);
    size = nr_pages * smmu->page_size;
    ret = aarch64_mmu_map(config->mmu, addr, addr, size, config->mmu_attr);

    return ret;
}

static errno_t map_register_region(const struct smmu500 *smmu,
                                   const struct smmu500_configuration *config)
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
    uint32_t id;

    /* probe SMMU_IDR0 */

    id = smmu500_gr0_read32(smmu, SMMU_IDR0);
    smmu->nr_stream_matches = EXTRACT_SMMU_IDR0_NUMSMRG(id);
    smmu->nr_context_fault_interrupts = EXTRACT_SMMU_IDR0_NUMIRPT(id);

    /* probe SMMU_IDR1 */

    id = smmu500_gr0_read32(smmu, SMMU_IDR1);

    if ((id & SMMU_IDR1_PAGESIZE) != 0) {
        smmu->page_size = 64 * 1024;
    } else {
        smmu->page_size = 4 * 1024;
    }

    d = EXTRACT_SMMU_IDR1_NUMPAGENDXB(id);
    smmu->nr_pages = 1 << (d + 1);

    smmu->nr_context_banks = EXTRACT_SMMU_IDR1_NUMCB(id);
    smmu->nr_s2_context_banks = EXTRACT_SMMU_IDR1_NUMS2CB(id);

    smmu->smmu_gr1_base = smmu->smmu_base + smmu->page_size;
    smmu->smmu_cb_base = smmu->smmu_base + (smmu->page_size * smmu->nr_pages);

    /* probe SMMU_IDR2 */

    id = smmu500_gr0_read32(smmu, SMMU_IDR2);
    if ((id & SMMU_IDR2_VMID16S) == 0) {
        smmu->vmid_size = 8;
    } else {
        smmu->vmid_size = 16;
    }
}

static errno_t initialize(struct smmu500 *smmu,
                          const struct smmu500_configuration *config)
{
    errno_t ret;

    smmu->smmu_base = config->smmu_base;
    probe_device(smmu);
    ret = map_register_region(smmu, config);
    if (ret == SUCCESS) {
        ret = initialize_device(smmu, config);
    }

    return ret;
}

static errno_t validate_parameters(const struct smmu500 *smmu,
                                   const struct smmu500_configuration *config)
{
    errno_t ret;

    if ((smmu != NULL) && (config != NULL) && (config->smmu_base != 0)
        && (config->mmu != NULL) && (config->mmu_attr != NULL)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t smmu500_initialize(struct smmu500 *smmu,
                           const struct smmu500_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(smmu, config);
    if (ret == SUCCESS) {
        ret = initialize(smmu, config);
    }

    return ret;
}
