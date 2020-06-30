/*
 * service/p2p_packet_ep/p2p_packet_ep_initialize.c
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

static errno_t ep_initialize(struct p2p_packet_ep *ep, const struct p2p_packet_ep_configuration *config)
{
    memset(ep, 0, sizeof(*ep));
    ep->peer = config->peer;
    ep->ops = config->ops;
    ep->arg = config->arg;
    ep->length = config->length;
    ep->interrupt_no = config->interrupt_no;

    return SUCCESS;
}

static errno_t validate_parameters(const struct p2p_packet_ep_configuration *config)
{
    errno_t ret;

    if (config->ops == NULL) {
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

errno_t p2p_packet_ep_initialize(struct p2p_packet_ep *ep, const struct p2p_packet_ep_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(config);
    if (ret == SUCCESS) {
        ret = ep_initialize(ep, config);
    }

    return ret;
}

