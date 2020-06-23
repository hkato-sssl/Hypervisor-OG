/*
 * service/p2p_packet/p2p_packet_send.c
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

static errno_t packet_send(struct p2p_packet *pkt, struct vpc *vpc)
{
    errno_t ret;
    uint32_t i;
    uint32_t n;
    uint64_t *buff;

    buff = pkt->peer->buff;
    n = pkt->peer->length / sizeof(uint64_t);
    for (i = 0; i < n; ++i) {
        buff[i] = vpc->regs[VPC_X0 + i];
    }

    memory_barrier();
    pkt->peer->status.empty = 1;
    memory_barrier();

    ret = p2p_packet_assert_interrupt(pkt->peer);

    return ret;
}

errno_t p2p_packet_send(struct p2p_packet *pkt, struct vpc *vpc)
{
    errno_t ret;

    if (pkt->peer->status.empty == 0) {
        ret = packet_send(pkt, vpc);
    } else {
        ret = -ENOBUFS;
    }

    return ret;
}

