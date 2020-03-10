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

#include "lib/bitmap.h"

/* defines */

#define IS_VALID_SIZE(sz)   (((sz) > 0) && ((sz) <= MAX_LIB_BITMAP_SIZE))

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* BITMAP_BITMAP_LOCAL_H */

