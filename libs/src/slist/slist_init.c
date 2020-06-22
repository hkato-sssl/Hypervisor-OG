/*
 * slist/slist_init.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include "lib/slist.h"

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

