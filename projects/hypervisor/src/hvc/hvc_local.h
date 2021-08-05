/*
 * hvc/hvc_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef SERVICE_HVC_HVC_LOCAL_H
#define SERVICE_HVC_HVC_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

bool hvc_match_service(void *element, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SERVICE_HVC_HVC_LOCAL_H */
