// SPDX-License-Identifier: Apache-2.0
/*
 * lib/list.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_LIST_H
#define LIB_LIST_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

struct list_element {
    struct list_element *prev;
    struct list_element *next;
    void *value;
};

struct list {
    struct list_element *head;
    struct list_element *tail;
    uint64_t num;
};

/* variables */

/* functions */

errno_t list_init(struct list *list);
errno_t list_append(struct list *list, struct list_element *e);
struct list_element *list_get_front(struct list *list);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_LIST_H */
