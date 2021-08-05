/*
 * lib/list.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/list.h"
#include "lib/system/errno.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

errno_t list_init(struct list *list)
{
    errno_t ret;

    if (list != NULL) {
        list->head = list->tail = NULL;
        list->num = 0;
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t list_append(struct list *list, struct list_element *e)
{
    errno_t ret;

    if ((list != NULL) && (e != NULL)) {
        if (list->tail == NULL) {
            e->prev = e->next = NULL;
            list->head = list->tail = e;
        } else {
            e->prev = list->tail;
            e->next = NULL;
            list->tail->next = e;
            list->tail = e;
        }
        ++(list->num);
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

struct list_element *list_get_front(struct list *list)
{
    struct list_element *e;

    if (list != NULL) {
        e = list->head;
        if (e != NULL) {
            list->head = e->next;
            if (e->next != NULL) {
                e->next->prev = NULL;
            } else {
                list->tail = NULL;
            }
            --(list->num);
        }
    } else {
        e = NULL;
    }

    return e;
}
