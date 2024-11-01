// SPDX-License-Identifier: Apache-2.0
/*
 * bitmap/bitmap_clear.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "bitmap_local.h"
#include "lib/bitmap.h"
#include "lib/system/errno.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t clear(uint8_t *map, uint32_t bit_no)
{
    map[bit_no / 8] &= ~(uint8_t)(1 << (bit_no & 7));

    return SUCCESS;
}

errno_t bitmap_clear(void *map, size_t map_size, uint32_t bit_no)
{
    errno_t ret;

    if ((map != NULL) && IS_VALID_SIZE(map_size) && ((bit_no / 8) < map_size)) {
        ret = clear(map, bit_no);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
