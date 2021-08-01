/*
 * aarch64/psci/psci_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef AARCH64_PSCI_PSCI_LOCAL_H
#define AARCH64_PSCI_PSCI_LOCAL_H

/* includes */

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

int32_t aarch64_psci(uint64_t request[4]);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* AARCH64_PSCI_PSCI_LOCAL_H */

