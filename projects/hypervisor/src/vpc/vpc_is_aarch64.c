/*
 * vpc/vpc_is_aarch64.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "vpc_local.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

bool vpc_is_aarch64(const struct vpc *vpc)
{
    return is_aarch64(vpc);
}