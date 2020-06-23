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

/* defines */

#define MAX_PACKET_SIZE     128     /* byte */
#define NR_MAX_PACKET_BUFF  (MAX_PACKET_SIZE / sizeof(uint64_t))

/* types */

struct vpc;
struct p2p_packet;

typedef errno_t (*p2p_packet_handler_t)(struct p2p_packet *pkt);

struct p2p_packet_ops {
    p2p_packet_handler_t    assert_interrupt;
};

struct p2p_packet {
    struct p2p_packet           *peer;
    const struct p2p_packet_ops *ops;
    void                        *arg;
    uint32_t                    length;
    uint16_t                    interrupt_no;
    struct {
        uint8_t                 empty;
    } status;
    uint64_t                    buff[NR_MAX_PACKET_BUFF];
};

struct p2p_packet_configuration {
    struct p2p_packet           *peer;
    const struct p2p_packet_ops *ops;
    void                        *arg;
    uint32_t                    length;
    uint16_t                    interrupt_no;
};

/* variables */

/* functions */

static inline errno_t p2p_packet_assert_interrupt(struct p2p_packet *pkt)
{
    errno_t ret;
    p2p_packet_handler_t handler;

    handler = pkt->ops->assert_interrupt;
    if (handler != NULL) {
        ret = (*handler)(pkt);
    } else {
        ret = SUCCESS;      /* no operation */
    }

    return ret;
}

errno_t p2p_packet_init(struct p2p_packet *pkt, const struct p2p_packet_configuration *config);
errno_t p2p_packet_send(struct p2p_packet *pkt, struct vpc *vpc);
errno_t p2p_packet_receive(struct p2p_packet *pkt, struct vpc *vpc);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SERVICE_P2P_PACKET_H */

