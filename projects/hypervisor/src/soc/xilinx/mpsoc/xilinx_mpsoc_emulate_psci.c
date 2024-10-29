// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/src/soc/xilinx/mpsoc/xilinx_mpsoc_emulate_psci.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/emulator/psci.h"
#include "hypervisor/emulator/simple_psci.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

static const struct psci_emulator_ops ops = {
    .psci_version = simple_psci_version,
    .cpu_on = simple_psci_cpu_on,
};

/* functions */

errno_t xilinx_mpsoc_emulate_psci(struct vpc *vpc)
{
    errno_t ret;

    ret = psci_call_aarch64_emulator_ops(vpc, &ops);

    return ret;
}
