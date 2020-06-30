/*
 * hypervisor/guest/shared_resource.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

/* variables */

struct p2p_packet_connector p2p_connector;

/* functions */

errno_t guest_initialize_shared_resource(void)
{
    errno_t ret;

    ret = p2p_packet_initialize_connector(&p2p_connector);

    return ret;
}
