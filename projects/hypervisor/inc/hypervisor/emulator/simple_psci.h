/*
 * hypervisor/emulator/simple_psci.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_EMULATOR_SIMPLE_PSCI_H
#define HYPERVISOR_EMULATOR_SIMPLE_PSCI_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "hypervisor/emulator/psci.h"
#include "lib/system/errno.h"

/* defines */

/* types */

struct vpc;

/* variables */

/* functions */

void simple_psci_initialize(struct psci_emulator_ops *ops);

errno_t simple_psci_version(struct vpc *vpc);
errno_t simple_psci_cpu_on(struct vpc *vpc);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_SIMPLE_PSCI_H */
