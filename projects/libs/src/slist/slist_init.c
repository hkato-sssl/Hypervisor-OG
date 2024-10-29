// SPDX-License-Identifier: Apache-2.0
/*
 * slist/slist_init.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "lib/slist.h"
#include <stddef.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void slist_init(struct slist *list)
{
    list->head = NULL;
    list->tail = NULL;
}
