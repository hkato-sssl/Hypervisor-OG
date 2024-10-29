// SPDX-License-Identifier: Apache-2.0
/*
 * service/p2p_packet/p2p_packet_call_handler.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/service/p2p_packet.h"
#include "lib/system/errno.h"
#include "p2p_packet_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

struct vpc;

/* prototypes */

/* variables */

/* functions */

errno_t p2p_packet_call_arrive_handler(struct p2p_packet_ep *ep)
{
    errno_t ret;

    if ((ep->ops != NULL) && (ep->ops->arrive != NULL)) {
        ret = (ep->ops->arrive)(ep);
    } else {
        ret = SUCCESS; /* no work */
    }

    return ret;
}

errno_t p2p_packet_call_empty_handler(struct p2p_packet_ep *ep)
{
    errno_t ret;

    if ((ep->ops != NULL) && (ep->ops->empty != NULL)) {
        ret = (ep->ops->empty)(ep);
    } else {
        ret = SUCCESS; /* no work */
    }

    return ret;
}
