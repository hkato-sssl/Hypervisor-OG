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

static errno_t assert_interrupt(struct vpc *vpc, struct p2p_packet_ep *ep);

/* variables */

const struct p2p_packet_ep_ops xilinx_mpsoc_p2p_packet_ep_ops = {
    .arrive = assert_interrupt,
    .empty = assert_interrupt,
};

/* functions */

static errno_t assert_interrupt(struct vpc *vpc, struct p2p_packet_ep *ep)
{
    errno_t ret;
    struct xilinx_mpsoc *mpsoc;

    mpsoc = ep->arg;
    if (vgic400_test_virtual_spi(&(mpsoc->vgic400), ep->interrupt_no)) {
        ret = vgic400_assert_virtual_spi_interrupt(vpc, &(mpsoc->vgic400), ep->interrupt_no);
    } else {
        ret = gic400_assert_spi(mpsoc->vgic400.gic, ep->interrupt_no);
    }

    return ret;
}

