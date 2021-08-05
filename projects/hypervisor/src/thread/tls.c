/*
 * hypervisor/thread/tls.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t thread_read_tls(uint32_t index)
{
    uint64_t d;
    uint64_t *p;

    p = (uint64_t *)aarch64_read_tpidr_el2();
    d = p[index];

    return d;
}

void thread_write_tls(uint32_t index, uint64_t d)
{
    uint64_t *p;

    p = (uint64_t *)aarch64_read_tpidr_el2();
    p[index] = d;
}
