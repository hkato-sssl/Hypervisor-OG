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

static errno_t test_executable_region(struct soc *soc, uintptr_t addr, size_t size)
{
    errno_t ret;
    uint32_t i;
    struct soc_device *dev;

    ret = -EPERM;
    for (i = 0; i < soc->nr_devices; ++i) {
        dev = soc->devices[i];
        if (dev->region.access.flag.exec != 0) {
            if ((dev->region.ipa <= addr) &&
                ((addr + size) <= (dev->region.ipa + dev->region.size))) {
                ret = SUCCESS;
                break;
            }
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

