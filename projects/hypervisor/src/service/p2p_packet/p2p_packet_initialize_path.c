/*
 * service/p2p_packet/p2p_packet_initialize_path.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/service/p2p_packet.h"
#include "lib/system/errno.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t p2p_packet_initialize_path(struct p2p_packet_path *path)
{
    memset(path, 0, sizeof(*path));

    return SUCCESS;
}
