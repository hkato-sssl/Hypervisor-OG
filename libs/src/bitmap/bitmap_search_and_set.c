/*
 * bitmap/bitmap_search_and_set.c
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

static errno_t search_and_set(uint32_t *bit_no, uint8_t *map, size_t map_size)
{
    errno_t ret;
    uint32_t i;
    uint32_t no;

    ret = -ENOMEM;
    for (i = 0; i < map_size; ++i) {
        if (map[i] != 0xff) {
            no = aarch64_clz(aarch64_rbit(~map[i]));
            map[i] |= 1 << no;
            *bit_no = no + (i * 8);
            ret = SUCCESS;
            break;
        }
    }

    return ret;
}

errno_t bitmap_search_and_set(uint32_t *bit_no, void *map, size_t map_size)
{
    errno_t ret;

    if ((bit_no != NULL) && (map != NULL) && IS_VALID_SIZE(map_size)) {
        ret = search_and_set(bit_no, map, map_size);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

