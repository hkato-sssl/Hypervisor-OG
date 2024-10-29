// SPDX-License-Identifier: Apache-2.0
/*
 * service/p2p_packet_ep/p2p_packet_receive.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "p2p_packet_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t receive(struct vpc *vpc, struct p2p_packet_ep *ep)
{
    errno_t ret;
    uint32_t i;
    uint32_t n;

    n = ep->length / sizeof(uint64_t);
    for (i = 0; i < n; ++i) {
        vpc->regs[VPC_X1 + i] = ep->buff[i];
    }

    memory_barrier();
    ep->status.empty = 1;
    ep->peer->event.peer_ready = 1;
    memory_barrier_sync();

    ret = p2p_packet_call_empty_handler(ep->peer);

    return ret;
}

errno_t p2p_packet_receive(struct vpc *vpc, struct p2p_packet_ep *ep)
{
    errno_t ret;

    if (ep->path != NULL) {
        p2p_packet_lock_path(ep);
        if (ep->status.empty == 0) {
            ret = receive(vpc, ep);
        } else {
            ret = -ENODATA;
        }
        p2p_packet_unlock_path(ep);
    } else {
        ret = -EPERM;
    }

    return ret;
}
