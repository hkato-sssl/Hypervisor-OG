/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/soc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

static errno_t test_executable_region(struct vpc *soc, uintptr_t addr);

/* variables */

struct soc_ops xilinx_mpsoc_ops = {
    .test_executable_region = test_executable_region,
};

/* functions */

static errno_t test_executable_region(struct vpc *soc, uintptr_t addr)
{
#if 0
    bool result;
    struct xilinx_mpsoc *chip;

    chip = soc->chip;
    if ((addr <= chip->ram.ipa) && (addr <= (chip->ram.ipa + chip->ram.size - 1))) {
        result = true;
    } else {
        result = false;
    }

    return result;
#else
    return -EINVAL;
#endif
}
