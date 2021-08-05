/*
 * bitmap/bitmap_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef BITMAP_BITMAP_LOCAL_H
#define BITMAP_BITMAP_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

#define IS_VALID_SIZE(sz) (((sz) > 0) && ((sz) <= MAX_LIB_BITMAP_SIZE))

/* types */

/* variables */

/* functions */

errno_t bitmap_search_zero(uint32_t *bit_no, const uint8_t *map,
                           size_t map_size, uint32_t offset);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* BITMAP_BITMAP_LOCAL_H */
