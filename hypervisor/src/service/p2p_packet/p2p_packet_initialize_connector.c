/*
 * service/p2p_packet/p2p_packet_initialize_connector.c
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

errno_t p2p_packet_initialize_connector(struct p2p_packet_connector *connector)
{
    memset(connector, 0, sizeof(*connector));

    return SUCCESS;
}

