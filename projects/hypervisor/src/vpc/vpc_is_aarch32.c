// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_is_aarch32.c
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

bool vpc_is_aarch32(const struct vpc *vpc)
{
    return is_aarch32(vpc);
}

bool vpc_is_aarch32a(const struct vpc *vpc)
{
    return is_aarch32a(vpc);
}

bool vpc_is_aarch32t(const struct vpc *vpc)
{
    return is_aarch32t(vpc);
}
