/*
 * lib/system/stdc/assert.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_STDC_ASSERT_H
#define LIB_SYSTEM_STDC_ASSERT_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/assert.h"
#include <stddef.h>

/* defines */

#ifdef NDEBUG
#define assert(exp)
#else
#define assert(exp)                                  \
    do {                                             \
        if (! (exp))                                 \
            system_assert(__FILE__, __LINE__, #exp); \
    } while (0)
#endif

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_STDC_ASSERT_H */
