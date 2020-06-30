/*
 * service/hvcs/hvcs_require.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/slist.h"
#include "lib/system/errno.h"
#include "hypervisor/service/hvcs.h"
#include "hvcs_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

bool hvcs_match_service(void *element, void *arg)
{
    bool result;
    struct hvcs_service *service;

    service = element;
    result = (service->id == (uint64_t)arg) ? true : false;

    return result;
}

