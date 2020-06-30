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

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"

/* defines */

#define MAX_PACKET_SIZE     128     /* byte */
#define NR_MAX_PACKET_BUFF  (MAX_PACKET_SIZE / sizeof(uint64_t))

/* types */

struct vpc;
struct p2p_packet_ep;
struct p2p_packet_connector;

typedef errno_t (*p2p_packet_handler_t)(struct p2p_packet_ep *ep);

struct p2p_packet_ep_ops {
    p2p_packet_handler_t    assert_interrupt;
};

struct p2p_packet_ep {
    struct p2p_packet_connector     *connector;
    struct p2p_packet_ep            *peer;
    const struct p2p_packet_ep_ops  *ops;
    void                            *arg;
    uint32_t                        length;
    uint16_t                        interrupt_no;
    struct {
        uint8_t                     empty;
    } status;
    uint64_t                        buff[NR_MAX_PACKET_BUFF];
};

struct p2p_packet_ep_configuration {
    const struct p2p_packet_ep_ops  *ops;
    void                            *arg;
    uint32_t                        length;
    uint16_t                        interrupt_no;
};

struct p2p_packet_connector {
    spin_lock_t                     lock;
    struct p2p_packet_ep            *eps[2];
};

/* variables */

/* functions */

errno_t p2p_packet_initialize_ep(struct p2p_packet_ep *ep, const struct p2p_packet_ep_configuration *config);
errno_t p2p_packet_initialize_connector(struct p2p_packet_connector *connector);
errno_t p2p_packet_initialize_trunk(struct p2p_packet_trunk *trunk, const struct p2p_packet_trunk_configuration *config);
errno_t p2p_packet_send(struct p2p_packet_ep *ep, struct vpc *vpc);
errno_t p2p_packet_receive(struct p2p_packet_ep *ep, struct vpc *vpc);
errno_t p2p_packet_assert_interrupt(struct p2p_packet_ep *ep);
errno_t p2p_packet_connect(struct p2p_packet_connector *connector, struct p2p_packet_ep *ep);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SERVICE_P2P_PACKET_H */

