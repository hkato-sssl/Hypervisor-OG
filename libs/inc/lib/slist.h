/*
 * lib/slist.h - Single-linked list library
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef LIB_SLIST_H
#define LIB_SLIST_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"

/* defines */

/* types */

struct slist_node {
    struct slist_node   *next;
    void                *element;
};

struct slist {
    struct slist_node   *head;
    struct slist_node   *tail;
};

typedef bool (*slist_match_func_t)(void *element, void *arg);

/* variables */

/* functions */

void slist_init(struct slist *list);
errno_t slist_append(struct slist *head, struct slist_node *node);
void *slist_search_element(const struct slist *list, slist_match_func_t match, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SLIST_H */

