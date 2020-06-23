/*
 * service/p2p_packet/p2p_packet_init.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t packet_init(struct p2p_packet *pkt, const struct p2p_packet_configuration *config)
{
    memset(pkt, 0, sizeof(*pkt));
    pkt->peer = config->peer;
    pkt->ops = config->ops;
    pkt->arg = config->arg;
    pkt->interrupt_no = config->interrupt_no;

    return SUCCESS;
}

static errno_t validate_parameters(const struct p2p_packet_configuration *config)
{
    errno_t ret;

    if (config->peer == NULL) {
        ret = -EINVAL;
    } else if (config->ops == NULL) {
        ret = -EINVAL;
    } else if ((config->length == 0) || (config->length > MAX_PACKET_SIZE)) {
        ret = -EINVAL;
    } else if ((config->length % sizeof(uint64_t)) != 0) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }
        
    return ret;
}

errno_t p2p_packet_init(struct p2p_packet *pkt, const struct p2p_packet_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(config);
    if (ret == SUCCESS) {
        ret = packet_init(pkt, config);
    }

    return ret;
}

