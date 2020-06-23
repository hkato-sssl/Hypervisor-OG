/*
 * service/p2p_packet_ep/p2p_packet_ep_send.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vpc_register.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t ep_send(struct p2p_packet_ep *ep, struct vpc *vpc)
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
    ep->peer->status.empty = 1;
    memory_barrier();

    ret = p2p_packet_assert_interrupt(ep->peer);

    return ret;
}

errno_t p2p_packet_ep_send(struct p2p_packet_ep *ep, struct vpc *vpc)
{
    errno_t ret;

    if (ep->peer->status.empty == 0) {
        ret = ep_send(ep, vpc);
    } else {
        ret = -ENOBUFS;
    }

    return ret;
}

