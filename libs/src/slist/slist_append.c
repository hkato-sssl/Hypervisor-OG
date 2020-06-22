/*
 * slist/slist_append.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/slist.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t append(struct slist *list, struct slist_node *node)
{
    node->next = NULL;

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    return SUCCESS;
}

errno_t slist_append(struct slist *list, struct slist_node *node)
{
    errno_t ret;

    if ((list != NULL) && (node != NULL)) {
        ret = append(list, node);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

