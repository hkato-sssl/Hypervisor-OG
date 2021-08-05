/*
 * bitmap/bitmap_set.c
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

static errno_t set(uint8_t *map, uint32_t bit_no)
{
    map[bit_no / 8] |= 1 << (bit_no & 7);

    return SUCCESS;
}

errno_t bitmap_set(void *map, size_t map_size, uint32_t bit_no)
{
    errno_t ret;

    if ((map != NULL) && IS_VALID_SIZE(map_size) && ((bit_no / 8) < map_size)) {
        ret = set(map, bit_no);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
