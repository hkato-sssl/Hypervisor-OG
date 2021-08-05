/*
 * service/p2p_packet_ep/p2p_packet_send.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vpc_register.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "p2p_packet_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t send(struct vpc *vpc, struct p2p_packet_ep *ep)
{
    errno_t ret;
    uint32_t i;
    uint32_t n;
    uint64_t *buff;

    buff = ep->peer->buff;
    n = ep->peer->length / sizeof(uint64_t);
    for (i = 0; i < n; ++i) {
        buff[i] = vpc->regs[VPC_X0 + i];
    }

    memory_barrier();
    ep->peer->status.empty = 0;
    ep->peer->event.data_ready = 1;
    memory_barrier_sync();

    ret = p2p_packet_call_arrive_handler(ep->peer);

    return ret;
}

errno_t p2p_packet_send(struct vpc *vpc, struct p2p_packet_ep *ep)
{
    errno_t ret;

    if (ep->path != NULL) {
        p2p_packet_lock_path(ep);
        if (ep->peer->status.empty != 0) {
            ret = send(vpc, ep);
        } else {
            ret = -ENOBUFS;
        }
        p2p_packet_unlock_path(ep);
    } else {
        ret = -EPERM;
    }

    return ret;
}
