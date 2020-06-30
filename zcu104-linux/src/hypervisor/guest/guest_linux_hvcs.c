/*
 * hypervisor/guest/guest_linux_hvcs.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/slist.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/service/hvcs.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

#define INTR_EP0        130
#define INTR_EP1        131

#define CMD_SEND        (((uint32_t)'S' << 8) | (uint32_t)'D')
#define CMD_RECV        (((uint32_t)'R' << 8) | (uint32_t)'V')

/* types */

/* prototypes */

static errno_t assert_interrupt(struct p2p_packet_ep *ep);

/* variables */

static struct p2p_packet_ep eps[2];
static struct hvcs_service service;

static struct p2p_packet_ep_ops ops = {
    .assert_interrupt = assert_interrupt,
};

/* functions */

static errno_t cmd_send(const struct insn *insn, struct p2p_packet_ep *ep)
{
    errno_t ret;

    ret = p2p_packet_ep_send(ep, insn->vpc);

    return ret;
}

static errno_t cmd_recv(const struct insn *insn, struct p2p_packet_ep *ep)
{
    errno_t ret;

    ret = p2p_packet_ep_receive(ep, insn->vpc);

    return ret;
}

static errno_t server(const struct insn *insn, void *arg)
{
    errno_t ret;
    struct p2p_packet_ep *ep;
    uint32_t cmd;
    uint32_t dev;

    /*
     *  ip0[31:16] - device ID
     *  ip0[15:0]  - command
     */

    cmd = (uint32_t)(insn->vpc->regs[VPC_IP0]);
    dev = cmd & BITS(15, 0);
    cmd >>= 16;

    if (dev <= 1) {
        ep = eps + dev;
        if (cmd == CMD_SEND) {
            ret = cmd_send(insn, ep);
        } else if (cmd == CMD_RECV) {
            ret = cmd_recv(insn, ep);
        } else {
            ret = -ENOTSUP;
        }
    } else {
        ret = -EINVAL;
    }

    insn->vpc->regs[VPC_X0] = (uint64_t)ret;

    return SUCCESS;
}

static errno_t assert_interrupt(struct p2p_packet_ep *ep)
{
    errno_t ret;
    struct xilinx_mpsoc *mpsoc;

    mpsoc = ep->arg;
    ret = gic400_assert_spi(mpsoc->vgic400.gic, ep->interrupt_no);

    return ret;
}

static errno_t init_ep(int no, struct xilinx_mpsoc *mpsoc, uint16_t interrupt_no)
{
    errno_t ret;
    int peer;
    struct p2p_packet_ep_configuration config;

    memset(&config, 0, sizeof(config));
    peer = (no == 0) ? 1 : 0;
    config.peer = eps + peer;
    config.ops = &ops;
    config.arg = mpsoc;
    config.length = 128;
    config.interrupt_no = interrupt_no;
    ret = p2p_packet_ep_initialize(&(eps[no]), &config);

    return ret;
}

static errno_t init_p2p_eps(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;
    
    ret = init_ep(0, mpsoc, INTR_EP0);
    if (ret == SUCCESS) {
        ret = init_ep(1, mpsoc, INTR_EP1);
    }

    return ret;
}

errno_t guest_linux_initialize_hvcs(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;
    struct hvcs_service_configuration config;

    ret = init_p2p_eps(mpsoc);

    if (ret == SUCCESS) {
        memset(&config, 0, sizeof(config));
        config.id = HVCS_SERVICE_ID(1, 'P', '1', '2', '8');
        config.server = server;
        config.arg = mpsoc;
        ret = hvcs_initialize_service(&service, &config);
    }

    if (ret == SUCCESS) {
        ret = hvcs_register_service(&(mpsoc->hvc_service_list), &service);
    }

    return ret;
}

