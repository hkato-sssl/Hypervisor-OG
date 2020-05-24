/*
 * soc/soc_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/mmu.h"
#include "hypervisor/vm.h"
#include "hypervisor/soc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t stage2_memattr(struct aarch64_stage2_attr *attr, const struct soc_device *dev)
{
    errno_t ret;

    ret = SUCCESS;
    switch (dev->region.memory_type) {
    case HYP_MMU_MT_NORMAL_NC:
        attr->memattr = STAGE2_MEMATTR_NORMAL_NC;
        break;
    case HYP_MMU_MT_NORMAL_WB:
        attr->memattr = STAGE2_MEMATTR_NORMAL_WB;
        break;
    case HYP_MMU_MT_NORMAL_WBWA:
        attr->memattr = STAGE2_MEMATTR_NORMAL_WB;
        break;
    case HYP_MMU_MT_DEVICE_nGnRnE:
        attr->memattr = STAGE2_MEMATTR_DEVICE_nGnRnE;
        break;
    case HYP_MMU_MT_DEVICE_nGnRE:
        attr->memattr = STAGE2_MEMATTR_DEVICE_nGnRE;
        break;
    case HYP_MMU_MT_DEVICE_nGRE:
        attr->memattr = STAGE2_MEMATTR_DEVICE_nGRE;
        break;
    case HYP_MMU_MT_DEVICE_GRE:
        attr->memattr = STAGE2_MEMATTR_DEVICE_GRE;
        break;
    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}

static errno_t create_stage2_attribute(struct aarch64_stage2_attr *attr, const struct soc_device *dev)
{
    errno_t ret;

    ret = SUCCESS;
    attr->af = 1;

    if (dev->region.access.flag.exec != 0) {
        attr->xn = 0;
    } else {
        attr->xn = 1;
    }

    if (dev->region.access.flag.read != 0) {
        if (dev->region.access.flag.write != 0) {
            attr->s2ap = STAGE2_S2AP_RW;
        } else {
            attr->s2ap = STAGE2_S2AP_RO;
        }
    } else {
        if (dev->region.access.flag.write != 0) {
            attr->s2ap = STAGE2_S2AP_WO;
        } else {
            attr->s2ap = STAGE2_S2AP_NONE;
        }
    }

    if (dev->region.shareability == SOC_NSH) {
        attr->sh = STAGE2_SH_NSH;
    } else if (dev->region.shareability == SOC_ISH) {
        attr->sh = STAGE2_SH_ISH;
    } else if (dev->region.shareability == SOC_OSH) {
        attr->sh = STAGE2_SH_OSH;
    } else {
        ret = -EINVAL;
    }

    if (ret == SUCCESS) {
        ret = stage2_memattr(attr, dev);
    }

    return ret;
}

static errno_t map_stage2(struct soc *soc)
{
    errno_t ret;
    uint16_t i;
    struct aarch64_stage2_attr attr;
    struct soc_device *dev;

    memset(&attr, 0, sizeof(attr));

    for (i = 0; i < soc->nr_devices; ++i) {
        dev = soc->devices[i];

        ret = create_stage2_attribute(&attr, dev);
        if (ret != SUCCESS) {
            break;
        }

        ret = aarch64_stage2_map(&(soc->vm.stage2), (void *)(dev->region.ipa), (void *)(dev->region.pa), dev->region.size, &attr);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static errno_t initialize(struct soc *soc, const struct soc_configuration *soc_config)
{
    errno_t ret;
    struct vm_configuration config;

    /* initialize struct soc */

    memset(soc, 0, sizeof(*soc));

    spin_lock_init(&(soc->lock));
    soc->chip = soc_config->chip;
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
        ret = map_stage2(soc);
    }

    return ret;
}

static errno_t validate_parameters(struct soc *soc, const struct soc_configuration *config)
{
    errno_t ret;

    if ((soc != NULL) && (config != NULL) && (config->chip != NULL) && (config->ops != NULL)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
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

