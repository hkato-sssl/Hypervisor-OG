/*
 * driver/aarch64/psci.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_PSCI_H
#define DRIVER_AARCH64_PSCI_H

/* includes */

#include "driver/arm/psci_common.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

int32_t aarch64_psci_cpu_off(void);
int32_t aarch64_psci_cpu_on(uint64_t target, void *entry, uintptr_t context);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_PSCI_H */
