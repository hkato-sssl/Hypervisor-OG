// SPDX-License-Identifier: Apache-2.0
/*
 * hvc/hvc_service_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/hvc.h"
#include "lib/system/errno.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hvc_service_initialize(struct hvc_service *service,
                               const struct hvc_service_configuration *config)
{
    memset(service, 0, sizeof(*service));

    service->id = config->id;
    service->server = config->server;
    service->arg = config->arg;

    return SUCCESS;
}
