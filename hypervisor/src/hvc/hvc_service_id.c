/*
 * hvc/hvc_service_id.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/hvc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint64_t hvc_service_id(uint16_t imm, const char *name)
{
    uint64_t id;
    const uint8_t *p;

    p = (const uint8_t *)name;
    id = ((uint64_t)imm << 32) |
         ((uint64_t)(p[0]) << 24) |
         ((uint64_t)(p[1]) << 16) |
         ((uint64_t)(p[2]) << 8) |
         (uint64_t)(p[3]);

    return id;
}
