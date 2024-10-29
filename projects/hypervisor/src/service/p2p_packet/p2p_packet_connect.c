// SPDX-License-Identifier: Apache-2.0
/*
 * service/p2p_packet/p2p_packet_connect.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/service/p2p_packet.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t connect(struct p2p_packet_path *path, struct p2p_packet_ep *ep)
{
    errno_t ret;

    if (path->eps[0] == NULL) {
        path->eps[0] = ep;
        ep->path = path;
        ret = SUCCESS;
    } else {
        if (path->eps[0]->length == ep->length) {
            path->eps[1] = ep;
            path->eps[0]->peer = ep;
            ep->peer = path->eps[0];
            ep->path = path;
            ret = SUCCESS;
        } else {
            ret = -EPERM;
        }
    }

    return ret;
}

static errno_t validate_parameters(struct p2p_packet_path *path,
                                   struct p2p_packet_ep *ep)
{
    errno_t ret;

    if ((path->eps[0] != NULL) && (path->eps[1] != NULL)) {
        ret = -EBUSY;
    } else if (ep->path != NULL) {
        ret = -EBUSY;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t p2p_packet_connect(struct p2p_packet_path *path,
                           struct p2p_packet_ep *ep)
{
    errno_t ret;

    spin_lock(&(path->lock));

    ret = validate_parameters(path, ep);
    if (ret == SUCCESS) {
        ret = connect(path, ep);
    }

    spin_unlock(&(path->lock));

    return ret;
}
