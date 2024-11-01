// SPDX-License-Identifier: Apache-2.0
/*
 * slist/slist_search_element.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "lib/slist.h"
#include "lib/system/errno.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void *search_element(const struct slist *list, slist_match_func_t match,
                            void *arg)
{
    void *element;
    struct slist_node *node;

    for (node = list->head; node != NULL; node = node->next) {
        if ((*match)(node->element, arg)) {
            break;
        }
    }

    element = (node != NULL) ? node->element : NULL;

    return element;
}

void *slist_search_element(const struct slist *list, slist_match_func_t match,
                           void *arg)
{
    void *element;

    if ((list != NULL) && (match != NULL)) {
        element = search_element(list, match, arg);
    } else {
        element = NULL;
    }

    return element;
}
