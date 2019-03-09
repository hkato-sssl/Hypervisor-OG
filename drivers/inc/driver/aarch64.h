/*
 * driver/aarch64.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_H
#define DRIVER_AARCH64_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* types */

/* variables */

/* functions */

bool aarch64_lock(void);
void aarch64_unlock(bool lock);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_H */

