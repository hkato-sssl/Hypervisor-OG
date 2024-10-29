// SPDX-License-Identifier: Apache-2.0
/*
 * soc/soc_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/mmu.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/mmu.h"
#include "hypervisor/parameter.h"
#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t create_stage2_attribute(struct aarch64_stage2_attr *attr,
                                       const struct soc_device_region *region)
{
    errno_t ret;
    uint8_t sh;
    uint8_t mt;

    memset(attr, 0, sizeof(*attr));

    attr->af = 1;

    if (region->access.flag.exec != 0) {
        attr->xn = 0;
    } else {
        attr->xn = 1;
    }

    if (region->access.flag.read != 0) {
        if (region->access.flag.write != 0) {
            attr->s2ap = STAGE2_S2AP_RW;
        } else {
            attr->s2ap = STAGE2_S2AP_RO;
        }
    } else {
        if (region->access.flag.write != 0) {
            attr->s2ap = STAGE2_S2AP_WO;
        } else {
            attr->s2ap = STAGE2_S2AP_NONE;
        }
    }

    attr->smmu.wacfg = SMMU_WACFG_WA;
    attr->smmu.racfg = SMMU_RACFG_RA;

    ret = hyp_mmu_stage2_shareability(&sh, region->shareability);
    if (ret == SUCCESS) {
        attr->sh = sh;
        ret = hyp_mmu_stage2_memory_type(&mt, region->memory_type);
        if (ret == SUCCESS) {
            attr->memattr = mt;
        }
    }

    return ret;
}

static errno_t map_stage2(struct soc *soc,
                          const struct soc_device_region *region)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    ret = create_stage2_attribute(&attr, region);
    if (ret == SUCCESS) {
        ret = aarch64_stage2_map(&(soc->vm.stage2), (void *)(region->ipa),
                                 (void *)(region->pa), region->size, &attr);
    }

    return ret;
}

static errno_t create_trap_condition(struct vm_region_trap *trap,
                                     const struct soc_device_region *region)
{
    errno_t ret;

    if (region->access.flag.exec == 0) {
        trap->condition.read = (region->access.flag.read == 0) ? true : false;
        trap->condition.write = (region->access.flag.write == 0) ? true : false;
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t create_region_trap(struct soc *soc,
                                  const struct soc_device_region *region)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = region->emulator->trap;
    memset(trap, 0, sizeof(*trap));

    ret = create_trap_condition(trap, region);
    if (ret == SUCCESS) {
        trap->memory.ipa = region->ipa;
        trap->memory.pa = region->pa;
        trap->memory.size = region->size;
        trap->memory.shareability = region->shareability;
        trap->memory.type = region->memory_type;
        trap->emulator.arg = region->emulator->arg;
        trap->emulator.handler = region->emulator->handler;
    }

    return ret;
}

static errno_t register_region_trap(struct soc *soc,
                                    const struct soc_device_region *region)
{
    errno_t ret;

    ret = create_region_trap(soc, region);
    if (ret == SUCCESS) {
        ret = vm_register_region_trap(&(soc->vm), region->emulator->trap);
    }

    return ret;
}

static errno_t map_region(struct soc *soc,
                          const struct soc_device_region *region)
{
    errno_t ret;

    if (region->emulator != NULL) {
        ret = register_region_trap(soc, region);
    } else {
        ret = map_stage2(soc, region);
    }

    return ret;
}

static errno_t map_el2(struct soc *soc, const struct soc_device_region *region)
{
    errno_t ret;
    uintptr_t va;
    struct aarch64_mmu_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 1;
    attr.af = 1;
    attr.sh = MMU_ATTR_SH_ISH;
    attr.ap21 = MMU_ATTR_AP_RO;
    attr.attrindx = HYP_MMU_MT_NORMAL_WB;

    va = region->pa + HYP_GUEST_REGION_BASE;
    ret = aarch64_mmu_map(soc->mmu, (void *)va, (void *)region->pa,
                          region->size, &attr);

    return ret;
}

static errno_t map_device(struct soc *soc, const struct soc_device *device)
{
    errno_t ret;
    uint16_t i;

    ret = SUCCESS;
    for (i = 0; i < device->nr_regions; ++i) {
        ret = map_region(soc, device->regions[i]);
        if (ret != SUCCESS) {
            break;
        }

        if (device->regions[i]->access.flag.exec != 0) {
            ret = map_el2(soc, device->regions[i]);
            if (ret != SUCCESS) {
                break;
            }
        }
    }

    return ret;
}

static errno_t map_devices(struct soc *soc)
{
    errno_t ret;
    uint16_t i;

    ret = SUCCESS;
    for (i = 0; i < soc->nr_devices; ++i) {
        ret = map_device(soc, soc->devices[i]);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static errno_t initialize(struct soc *soc,
                          const struct soc_configuration *soc_config)
{
    errno_t ret;
    struct vm_configuration config;

    /* initialize struct soc */

    memset(soc, 0, sizeof(*soc));

    soc->chip = soc_config->chip;
    soc->mmu = soc_config->mmu;
    soc->ops = soc_config->ops;
    soc->nr_devices = soc_config->nr_devices;
    soc->devices = soc_config->devices;

    /* initialize struct vm */

    memset(&config, 0, sizeof(config));

    config.soc = soc;
    config.nr_procs = soc_config->nr_procs;
    config.stage2 = soc_config->stage2;

    ret = vm_initialize(&(soc->vm), &config);
    if (ret == SUCCESS) {
        ret = map_devices(soc);
    }

    return ret;
}

static errno_t validate_parameters(struct soc *soc,
                                   const struct soc_configuration *config)
{
    errno_t ret;

    if (config->chip == NULL) {
        ret = -EINVAL;
    } else if (config->mmu == NULL) {
        ret = -EINVAL;
    } else if (config->ops == NULL) {
        ret = -EINVAL;
    } else if (config->nr_devices == 0) {
        ret = -EINVAL;
    } else if (config->devices == NULL) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t soc_initialize(struct soc *soc, const struct soc_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(soc, config);
    if (ret == SUCCESS) {
        ret = initialize(soc, config);
    }

    return ret;
}
