/*
 * service/hvcs/hvcs_initialize_service.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "hypervisor/service/hvcs.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hvcs_initialize_service(struct hvcs_service *service, const struct hvcs_service_configuration *config)
{
    memset(service, 0, sizeof(*service));

    service->id = config->id;
    service->server = config->server;
    service->arg = config->arg;

    return SUCCESS;
}

