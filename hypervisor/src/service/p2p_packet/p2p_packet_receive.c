/*
 * service/p2p_packet/p2p_packet_receive.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "hypervisor/vpc.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t packet_receive(struct p2p_packet *pkt, struct vpc *vpc)
{
    errno_t ret;
    uint32_t i;
    uint32_t n;

    n = pkt->length / sizeof(uint64_t);
    for (i = 0; i < n; ++i) {
        vpc->regs[VPC_X0 + i] = pkt->buff[i];
    }

    memory_barrier();
    pkt->status.empty = 0;
    memory_barrier();

    ret = p2p_packet_assert_interrupt(pkt->peer);

    return ret;
}

errno_t p2p_packet_receive(struct p2p_packet *pkt, struct vpc *vpc)
{
    errno_t ret;

    if (pkt->status.empty != 0) {
        ret = packet_receive(pkt, vpc);
    } else {
        ret = -ENODATA;
    }

    return ret;
}

