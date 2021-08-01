/*
 * service/p2p_packet/p2p_packet_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef SERVICE_P2P_PACKET_P2P_PACKET_LOCAL_H
#define SERVICE_P2P_PACKET_P2P_PACKET_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

struct vpc;

/* variables */

/* functions */

errno_t p2p_packet_call_arrive_handler(struct p2p_packet_ep *ep);
errno_t p2p_packet_call_empty_handler(struct p2p_packet_ep *ep);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SERVICE_P2P_PACKET_P2P_PACKET_LOCAL_H */

