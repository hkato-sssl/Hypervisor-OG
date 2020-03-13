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

static errno_t search0_offset(uint32_t *bit_no, const uint8_t *map, size_t map_size, uint32_t offset)
{
    errno_t ret;
    uint8_t d;
    uint32_t i;

    ret = -ENOMEM;
    for (i = offset; i < map_size; ++i) {
        d = map[i];
        if (d != 0xff) {
            *bit_no = aarch64_clz(aarch64_rbit(~d)) + (offset * 8);
            ret = SUCCESS;
            break;
        }
    }

    return ret;
}

static errno_t search0_bit(uint32_t *bit_no, const uint8_t *map, size_t map_size, uint32_t offset, uint32_t bit)
{
    errno_t ret;
    uint8_t d;
    uint8_t mask;

    mask = ~(0xff << bit); 
    d = map[offset] | mask;
    if (d != 0xff) {
        *bit_no = aarch64_clz(aarch64_rbit(~d)) + (offset * 8);
        ret = SUCCESS;
    } else {
        ret = search0_offset(bit_no, map, map_size, (offset + 1));
    }

    return ret;
}

static errno_t search0(uint32_t *bit_no, const void *map, size_t map_size, uint32_t start_bit)
{
    errno_t ret;
    uint32_t bit;
    uint32_t offset;

    offset = start_bit / 8;
    bit = start_bit % 8;
    if (offset < map_size) {
        if (bit == 0) {
            ret = search0_offset(bit_no, map, map_size, offset);
        } else {
            ret = search0_bit(bit_no, map, map_size, offset, bit);
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t bitmap_search0(uint32_t *bit_no, const void *map, size_t map_size, uint32_t start_bit)
{
    errno_t ret;

    if ((bit_no != NULL) && (map != NULL) && IS_VALID_SIZE(map_size)) {
        ret = search0(bit_no, map, map_size, start_bit);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

