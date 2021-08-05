/*
 * aarch64/psci/aarch64_psci_cpu_on.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/psci.h"
#include "psci_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

int32_t aarch64_psci_cpu_on(uint64_t target, void *entry, uintptr_t context)
{
    uint64_t request[4];

    request[0] = PSCI_AARCH64_CPU_ON;
    request[1] = target;
    request[2] = (uint64_t)entry;
    request[3] = (uint64_t)context;

    return aarch64_psci(request);
}
