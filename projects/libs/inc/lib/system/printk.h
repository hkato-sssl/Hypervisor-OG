/*
 * libs/system/printk.h
 *
 * (C) 2018 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_PRINTK_H
#define LIB_SYSTEM_PRINTK_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t printk(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_PRINTK_H */
