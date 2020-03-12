/*
 * bitmap/bitmap_search0.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "lib/bitmap.h"
#include "bitmap_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t search0(uint32_t *bit_no, const uint8_t *map, size_t map_size)
{
    errno_t ret;
    uint32_t i;

    ret = -ENOMEM;
    for (i = 0; i < map_size; ++i) {
        if (map[i] != 0xff) {
            *bit_no = aarch64_clz(aarch64_rbit(~map[i])) + (i * 8);
            ret = SUCCESS;
            break;
        }
    }

    return ret;
}

errno_t bitmap_search0(uint32_t *bit_no, const void *map, size_t map_size)
{
    errno_t ret;

    if ((bit_no != NULL) && (map != NULL) && IS_VALID_SIZE(map_size)) {
        ret = search0(bit_no, map, map_size);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

