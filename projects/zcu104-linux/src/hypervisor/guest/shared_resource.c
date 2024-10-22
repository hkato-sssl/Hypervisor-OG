/*
 * hypervisor/guest/shared_resource.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/service/p2p_packet.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

struct p2p_packet_path guest_to_guest_path;

/* functions */

errno_t guest_initialize_shared_resource(void)
{
    errno_t ret;

    ret = p2p_packet_initialize_path(&guest_to_guest_path);

    return ret;
}
