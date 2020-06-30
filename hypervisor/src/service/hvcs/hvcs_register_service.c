/*
 * service/hvcs/hvcs_register_service.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/slist.h"
#include "lib/system/errno.h"
#include "hypervisor/service/hvcs.h"
#include "hvcs_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hvcs_register_service(struct slist *list, struct hvcs_service *service)
{
    errno_t ret;
    void *id;
    void *element;

    id = (void *)(uintptr_t)(service->id);
    element = slist_search_element(list, hvcs_match_service, id);
    if (element == NULL) {
        service->node.element = service;
        ret = slist_append(list, &(service->node));
    } else {
        ret = -EBUSY;
    }

    return ret;
}

