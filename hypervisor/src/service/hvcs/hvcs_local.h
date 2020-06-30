/*
 * service/hvcs/hvcs_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef SERVICE_HVCS_HVCS_LOCAL_H
#define SERVICE_HVCS_HVCS_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"

/* defines */

/* types */

/* variables */

/* functions */

bool hvcs_match_service(void *element, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SERVICE_HVCS_HVCS_LOCAL_H */

