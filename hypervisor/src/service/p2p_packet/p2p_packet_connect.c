/*
 * service/p2p_packet/p2p_packet_connect.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t connect(struct p2p_packet_connector *connector, struct p2p_packet_ep *ep)
{
    errno_t ret;

    if (connector->eps[0] == NULL) {
        connector->eps[0] = ep;
        ep->connector = connector;
        ret = SUCCESS;
    } else {
        if (connector->eps[0]->length == ep->length) {
            connector->eps[1] = ep;
            connector->eps[0]->peer = ep;
            ep->peer = connector->eps[0];
            ep->connector = connector;
            ret = SUCCESS;
        } else {
            ret = -EPERM;
        }
    }

    return ret;
}

static errno_t validate_parameters(struct p2p_packet_connector *connector, struct p2p_packet_ep *ep)
{
    errno_t ret;

    if ((connector->eps[0] != NULL) && (connector->eps[1] != NULL)) {
        ret = -EBUSY;
    } else if (ep->connector != NULL) {
        ret = -EBUSY;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t p2p_packet_connect(struct p2p_packet_connector *connector, struct p2p_packet_ep *ep)
{
    errno_t ret;

    spin_lock(&(connector->lock));

    ret = validate_parameters(connector, ep);
    if (ret == SUCCESS) {
        ret = connect(connector, ep);
    }

    spin_unlock(&(connector->lock));

    return ret;
}
