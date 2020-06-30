/*
 * service/p2p_packet_ep/p2p_packet_receive.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/spin_lock.h"
#include "hypervisor/vpc.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t receive(struct p2p_packet_ep *ep, struct vpc *vpc)
{
    errno_t ret;
    uint32_t i;
    uint32_t n;

    n = ep->length / sizeof(uint64_t);
    for (i = 0; i < n; ++i) {
        vpc->regs[VPC_X1 + i] = ep->buff[i];
    }

    memory_barrier();
    ep->status.empty = 0;
    memory_barrier();

    ret = p2p_packet_assert_interrupt(ep->peer);

    return ret;
}

errno_t p2p_packet_receive(struct p2p_packet_ep *ep, struct vpc *vpc)
{
    errno_t ret;

    if (ep->connector != NULL) {
        spin_lock(&(ep->connector->lock));
        if (ep->status.empty != 0) {
            ret = receive(ep, vpc);
        } else {
            ret = -ENODATA;
        }
        spin_unlock(&(ep->connector->lock));
    } else {
        ret = -EPERM;
    }

    return ret;
}
