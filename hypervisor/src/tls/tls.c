/*
 * hypervisor/tls.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "driver/aarch64/system_register.h"
#include "hypervisor/tls.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t tls_read(uint32_t index)
{
    uint64_t d;
    uint64_t *p;

    p = (uint64_t *)aarch64_read_tpidr_el2();
    d = p[index];

    return d;
}

void tls_write(uint32_t index, uint64_t d)
{
    uint64_t *p;

    p = (uint64_t *)aarch64_read_tpidr_el2();
    p[index] = d;
}

