// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/soc/xilinx/mpsoc/xilinx_mpsoc_p2p_packet_ep_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

static errno_t assert_interrupt(struct p2p_packet_ep *ep);

/* variables */

const struct p2p_packet_ep_ops xilinx_mpsoc_p2p_packet_ep_ops = {
    .arrive = assert_interrupt,
    .empty = assert_interrupt,
};

/* functions */

static errno_t assert_interrupt(struct p2p_packet_ep *ep)
{
    errno_t ret;
    struct xilinx_mpsoc *mpsoc;

    mpsoc = ep->owner;
    ret = gic400_assert_spi(mpsoc->vgic400.gic, ep->interrupt_no);

    return ret;
}
