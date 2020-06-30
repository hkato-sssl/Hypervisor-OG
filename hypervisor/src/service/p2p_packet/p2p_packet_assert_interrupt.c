/*
 * service/p2p_packet/p2p_packet_assert_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t p2p_packet_assert_interrupt(struct p2p_packet_ep *ep)
{
    errno_t ret;
    p2p_packet_handler_t handler;

    handler = ep->ops->assert_interrupt;
    if (handler != NULL) {
        ret = (*handler)(ep);
    } else {
        ret = SUCCESS;      /* no operation */
    }

    return ret;
}

