/*
 * hvc/p128/hvc_p128_server.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/hvc.h"
#include "hypervisor/hvc/p128.h"

/* defines */

#define P128_CMD(c0, c1)        (((uint16_t)(c0) << 8) | (uint16_t)(c1))
#define P128_CMD_NI             P128_CMD('N', 'I')
#define P128_CMD_WR             P128_CMD('W', 'R')
#define P128_CMD_RD             P128_CMD('R', 'D')
#define P128_CMD_GI             P128_CMD('G', 'I')
#define P128_CMD_GS             P128_CMD('G', 'S')
#define P128_CMD_GE             P128_CMD('G', 'E')

#define P128_STATUS_DATA_READY  BIT(0)
#define P128_STATUS_TX_EMPTY    BIT(1)

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t cmd_number_of_interfaces(struct vpc *vpc, const struct hvc_p128_service *service)
{
    vpc->regs[VPC_X1] = service->nr_eps;

    return SUCCESS;
}

static errno_t cmd_write(struct vpc *vpc, const struct hvc_p128_service *service, struct p2p_packet_ep *ep)
{
    errno_t ret;

    ret = p2p_packet_send(vpc, ep);
    
    return ret;
}

static errno_t cmd_read(struct vpc *vpc, const struct hvc_p128_service *service, struct p2p_packet_ep *ep)
{
    errno_t ret;

    ret = p2p_packet_receive(vpc, ep);

    return ret;
}

static errno_t cmd_get_interrupt_no(struct vpc *vpc, const struct hvc_p128_service *service, struct p2p_packet_ep *ep)
{
    vpc->regs[VPC_X1] = ep->interrupt_no;

    return SUCCESS;
}

static errno_t cmd_get_status(struct vpc *vpc, const struct hvc_p128_service *service, struct p2p_packet_ep *ep)
{
    uint64_t d;

    if (ep->status.empty == 0) {
        d = P128_STATUS_DATA_READY;
    } else {
        d = 0;
    }

    if (ep->peer->status.empty != 0) {
        d |= P128_STATUS_TX_EMPTY;
    }

    vpc->regs[VPC_X1] = d;

    return SUCCESS;
}

static errno_t cmd_get_event(struct vpc *vpc, const struct hvc_p128_service *service, struct p2p_packet_ep *ep)
{
    uint64_t d;

    if (ep->event.data_ready != 0) {
        ep->event.data_ready = 0;
        d = P128_STATUS_DATA_READY;
    } else {
        d = 0;
    }

    if (ep->event.peer_ready != 0) {
        ep->event.peer_ready = 0;
        d |= P128_STATUS_TX_EMPTY;
    }

    vpc->regs[VPC_X1] = d;

    return SUCCESS;
}

static errno_t p128_interface_command(struct vpc *vpc, const struct hvc_p128_service *service, struct p2p_packet_ep *ep, uint16_t command)
{
    errno_t ret;

    switch (command) {
    case P128_CMD_WR:
        ret = cmd_write(vpc, service, ep);
        break;
    case P128_CMD_RD:
        ret = cmd_read(vpc, service, ep);
        break;
    case P128_CMD_GI:
        ret = cmd_get_interrupt_no(vpc, service, ep);
        break;
    case P128_CMD_GS:
        ret = cmd_get_status(vpc, service, ep);
        break;
    case P128_CMD_GE:
        ret = cmd_get_event(vpc, service, ep);
        break;
    default:
        ret = -ENOTSUP;
        break;
    }

    return ret;
}

errno_t hvc_p128_server(const struct insn *insn, const struct hvc_service *service)
{
    errno_t ret;
    uint64_t ip0;
    uint16_t command;
    uint16_t ifno;
    struct hvc_p128_service *p128;
    struct p2p_packet_ep *ep;

    p128 = service->arg;
    ip0 = insn->vpc->regs[VPC_IP0];
    command = (uint16_t)(ip0 >> 16);

    if (command == P128_CMD_NI) {
        ret = cmd_number_of_interfaces(insn->vpc, p128);
    } else {
        ifno = (uint16_t)ip0;
        if (ifno < p128->nr_eps) {
            ep = p128->eps[ifno];
            ret = p128_interface_command(insn->vpc, p128, ep, command);
        } else {
            ret = -EINVAL;
        }
    }

    ret = hvc_set_result(insn->vpc, HVC_ERRNO(ret));

    return ret;
}

