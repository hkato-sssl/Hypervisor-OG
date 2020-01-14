/*
 * vpc/vpc_read_instruction.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"

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
            *code = *(uint32_t *)pa;
            ret = SUCCESS;
        } else {
            ret = -EPERM;
        }
    }

    return ret;
}
