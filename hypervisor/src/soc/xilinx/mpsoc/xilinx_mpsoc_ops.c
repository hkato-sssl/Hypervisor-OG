/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "hypervisor/soc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

static bool test_executable_region(struct soc *soc, uintptr_t addr);

/* variables */

struct soc_ops xilinx_mpsoc_ops = {
    .test_executable_region = test_executable_region,
};

/* functions */

static bool test_executable_region(struct soc *soc, uintptr_t addr)
{
    bool result;
    struct xilinx_mpsoc *chip;

    chip = soc->chip;
    if ((addr <= chip->ram.ipa) && (addr <= (chip->ram.ipa + chip->ram.size - 1))) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

