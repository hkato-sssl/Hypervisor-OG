/*
 * aarch64/psci/aarch64_psci_cpu_off.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/psci.h"
#include "psci_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

int32_t aarch64_psci_cpu_off(void)
{
    int32_t ret;
    uint64_t request[4];

    request[0] = PSCI_AARCH64_CPU_OFF;
    request[1] = 0;
    request[2] = 0;
    request[3] = 0;
    ret = aarch64_psci(request);

    return ret;
}
