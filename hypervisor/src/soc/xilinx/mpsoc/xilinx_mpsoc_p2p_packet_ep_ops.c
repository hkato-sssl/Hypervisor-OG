/*
 * hypervisor/soc/xilinx/mpsoc/xilinx_mpsoc_p2p_packet_ep_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/service/p2p_packet.h"

/* defines */

/* types */

/* prototypes */

static errno_t assert_interrupt(struct p2p_packet_ep *ep);

/* variables */

const struct p2p_packet_ep_ops xilinx_mpsoc_p2p_packet_ep_ops = {
    .assert_interrupt = assert_interrupt,
};

/* functions */

static errno_t assert_interrupt(struct p2p_packet_ep *ep)
{
    errno_t ret;
    struct xilinx_mpsoc *mpsoc;

    mpsoc = ep->arg;
    ret = gic400_assert_spi(mpsoc->vgic400.gic, ep->interrupt_no);

    return ret;
}

