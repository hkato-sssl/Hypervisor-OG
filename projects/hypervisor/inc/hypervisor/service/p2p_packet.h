// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/service/p2p/p2p_packet.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_SERVICE_P2P_PACKET_H
#define HYPERVISOR_SERVICE_P2P_PACKET_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include <stdint.h>

/* defines */

#define MAX_PACKET_SIZE 128 /* byte */
#define NR_PACKET_BUFFS (MAX_PACKET_SIZE / sizeof(uint64_t))

/* types */

struct vpc;
struct p2p_packet_ep;
struct p2p_packet_path;

typedef errno_t (*p2p_packet_handler_t)(struct p2p_packet_ep *ep);

struct p2p_packet_ep_ops {
    p2p_packet_handler_t arrive; /* A packet has arrived. */
    p2p_packet_handler_t empty;  /* The peer has received a packet. */
};

struct p2p_packet_ep {
    void *owner;
    struct p2p_packet_path *path;
    struct p2p_packet_ep *peer;
    const struct p2p_packet_ep_ops *ops;
    void *arg;
    uint32_t length;
    uint16_t interrupt_no;
    struct {
        uint8_t empty;
    } status;
    struct {
        uint8_t peer_ready; /* The peer can receive a packet */
        uint8_t data_ready; /* The EP has received a packet */
    } event;
    uint64_t buff[NR_PACKET_BUFFS];
};

struct p2p_packet_ep_configuration {
    void *owner;
    const struct p2p_packet_ep_ops *ops;
    void *arg;
    uint32_t length;
    uint16_t interrupt_no;
};

struct p2p_packet_path {
    spin_lock_t lock;
    struct p2p_packet_ep *eps[2];
};

/* variables */

/* functions */

errno_t
p2p_packet_initialize_ep(struct p2p_packet_ep *ep,
                         const struct p2p_packet_ep_configuration *config);
errno_t p2p_packet_initialize_path(struct p2p_packet_path *path);
errno_t p2p_packet_send(struct vpc *vpc, struct p2p_packet_ep *ep);
errno_t p2p_packet_receive(struct vpc *vpc, struct p2p_packet_ep *ep);
errno_t p2p_packet_connect(struct p2p_packet_path *path,
                           struct p2p_packet_ep *ep);

static inline void p2p_packet_lock_path(struct p2p_packet_ep *ep)
{
    spin_lock(&(ep->path->lock));
}

static inline void p2p_packet_unlock_path(struct p2p_packet_ep *ep)
{
    spin_unlock(&(ep->path->lock));
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SERVICE_P2P_PACKET_H */
