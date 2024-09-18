/*
 * emulator/simple_psci/simple_psci_init.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "hypervisor/emulator/psci.h"
#include "hypervisor/emulator/simple_psci.h"
#include "lib/system/errno.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static const struct psci_emulator_ops simple_psci_emulator_ops = {
    .psci_version = simple_psci_version,
    .cpu_on = simple_psci_cpu_on,
};

/* functions */

void simple_psci_initialize(struct psci_emulator_ops *ops)
{
    *ops = simple_psci_emulator_ops;
}