// SPDX-License-Identifier: Apache-2.0
/*
 * hvc/hvc_match_service.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hvc_local.h"
#include "hypervisor/hvc.h"
#include "lib/slist.h"
#include "lib/system/errno.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

bool hvc_match_service(void *element, void *arg)
{
    bool result;
    struct hvc_service *service;

    service = element;
    result = (service->id == (uint64_t)arg) ? true : false;

    return result;
}
