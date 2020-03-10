/*
 * lib/bitmap.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef LIB_BITMAP_H
#define LIB_BITMAP_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

#define MAX_LIB_BITMAP_SIZE     0x20000000

/* types */

/* variables */

/* functions */

errno_t bitmap_search0(uint32_t *bit_no, const void *map, size_t map_size);
errno_t bitmap_set(void *map, size_t map_size, uint32_t bit_no);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_BITMAP_H */

