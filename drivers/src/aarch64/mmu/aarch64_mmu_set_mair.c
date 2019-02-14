/*
 * aarch64/mmu/aarch64_mmu_set_mair.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/mmu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t mmu_set_mair(uint8_t const *attributes)
{
    int i;
    uint64_t d;
    
    d = 0;

    for (i = 0; i < NR_MAIR_ATTRS; ++i) {
        d |= (uint64_t)(attributes[i]) << (i * 8);
    }

    aarch64_write_mair(d);

    return SUCCESS;
}

errno_t aarch64_mmu_set_mair(uint8_t const *attributes)
{
    errno_t ret;

    if (attributes != NULL) {
        ret = mmu_set_mair(attributes);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

