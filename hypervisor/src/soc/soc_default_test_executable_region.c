/*
 * soc/soc_default_test_executable_region.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t test_device_regions(struct soc *soc, uintptr_t addr, size_t size, const struct soc_device *dev)
{
    errno_t ret;
    uint32_t i;
    const struct soc_device_region *region;

    ret = -EPERM;
    for (i = 0; i < dev->nr_regions; ++i) {
        region = dev->regions[i];
        if (region->access.flag.exec != 0) {
            if ((region->ipa <= addr) &&
                ((addr + size - 1) <= (region->ipa + region->size - 1))) {
                ret = SUCCESS;
                break;
            }
        }
    }

    return ret;
}

static errno_t test_executable_region(struct soc *soc, uintptr_t addr, size_t size)
{
    errno_t ret;
    uint32_t i;
    struct soc_device *dev;

    ret = -EPERM;
    for (i = 0; i < soc->nr_devices; ++i) {
        dev = soc->devices[i];
        ret = test_device_regions(soc, addr, size, dev);
        if (ret == SUCCESS) {
            break;
        }
    }

    return ret;
}

errno_t soc_default_test_executable_region(struct soc *soc, uintptr_t addr, size_t size)
{
    errno_t ret;

    if (soc->nr_devices == 0) {
        ret = -EPERM;
    } if (size == 0) {
        ret = -EINVAL;
    } else {
        ret = test_executable_region(soc, addr, size);
    }
    
    return ret;
}

