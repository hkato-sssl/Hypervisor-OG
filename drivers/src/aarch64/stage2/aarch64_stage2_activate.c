/*
 * aarch64/stage2/aarch64_stage2_activate.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/stage2.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t activate(struct aarch64_stage2 *st2)
{
    uint64_t d;

    d = (uint64_t)(st2->vmid) << 48;
    d |= (uint64_t)(st2->base.addr);
    aarch64_write_vttbr_el2(d);
    aarch64_write_vtcr_el2(st2->vtcr_el2);

    return SUCCESS;
}

errno_t aarch64_stage2_activate(struct aarch64_stage2 *st2)
{
    errno_t ret;

    if (st2 != NULL) {
        ret = activate(st2);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

