/*
 * hvc/hvc_service_append_slist.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/slist.h"
#include "lib/system/errno.h"
#include "hypervisor/hvc.h"
#include "hvc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hvc_service_append_slist(struct slist *list, struct hvc_service *service)
{
    errno_t ret;
    void *id;
    void *element;

    id = (void *)(uintptr_t)(service->id);
    element = slist_search_element(list, hvc_match_service, id);
    if (element == NULL) {
        service->node.element = service;
        ret = slist_append(list, &(service->node));
    } else {
        ret = -EBUSY;
    }

    return ret;
}

