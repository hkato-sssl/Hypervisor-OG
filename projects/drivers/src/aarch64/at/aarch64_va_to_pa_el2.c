/*
 * aarch64/at/aarch64_va_to_pa_el2.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/at.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t aarch64_va_to_pa_el2(uintptr_t *pa, uintptr_t va)
{
    errno_t ret;
    uint64_t d;

    d = aarch64_at_s1e2r(va);
    if ((d & BIT(0)) == 0) {
        d = aarch64_at_s1e2w(va);
        if ((d & BIT(0)) == 0) {
            *pa = (d & BITS(47, 12)) | (va & BITS(11, 0));
            ret = SUCCESS;
        } else {
            ret = -EPERM;
        }
    } else {
        ret = -EPERM;
    }

    return ret;
}
