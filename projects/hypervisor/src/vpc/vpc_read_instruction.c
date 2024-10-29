// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_read_instruction.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/parameter.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vpc_read_instruction(const struct vpc *vpc, uint32_t *code, uint64_t va)
{
    errno_t ret;
    uint64_t pa;

    ret = vpc_va_to_pa_r(vpc, &pa, va);
    if (ret == SUCCESS) {
        if (IS_ALIGNED(pa, 4)) {
            *code = *(uint32_t *)(HYP_GUEST_REGION_BASE + pa);
            ret = SUCCESS;
        } else {
            ret = -EPERM;
        }
    }

    return ret;
}
