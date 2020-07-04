/*
 * hypervisor/guest/guest_linux_hvc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/slist.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/hvc/p128.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

#define DEV_NAME        "P128"
#define HVC_IMM         1

#define INTR_EP0        130
#define INTR_EP1        131

/* types */

/* prototypes */

static errno_t assert_interrupt(struct p2p_packet_ep *ep);

/* variables */

extern struct p2p_packet_path p2p_path;

static struct p2p_packet_ep eps[2];
static struct p2p_packet_ep *p128_eps[2] = {
    &(eps[0]),
    &(eps[1]),
};
static struct hvc_p128_service p128;

static struct p2p_packet_ep_ops ops = {
    .arrive = assert_interrupt,
    .empty = assert_interrupt,
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

static errno_t init_ep(int no, struct xilinx_mpsoc *mpsoc, uint16_t interrupt_no)
{
    errno_t ret;
    struct p2p_packet_ep_configuration config;

    memset(&config, 0, sizeof(config));
    config.ops = &ops;
    config.arg = mpsoc;
    config.length = 128;
    config.interrupt_no = interrupt_no;
    ret = p2p_packet_initialize_ep(&(eps[no]), &config);
    if (ret == SUCCESS) {
        ret = p2p_packet_connect(&p2p_path, &(eps[no]));
    }

    return ret;
}

static errno_t init_p128_service(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;
    struct hvc_p128_service_configuration config;

    memset(&config, 0, sizeof(config));
    config.imm = HVC_IMM;
    config.name[0] = DEV_NAME[0];
    config.name[1] = DEV_NAME[1];
    config.name[2] = DEV_NAME[2];
    config.name[3] = DEV_NAME[3];
    config.arg = mpsoc;
    config.nr_eps = 2;
    config.eps = p128_eps;
    ret = hvc_p128_service_initialize(&p128, &config);

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

errno_t guest_linux_initialize_hvc(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;

    ret = init_p2p_eps(mpsoc);

    if (ret == SUCCESS) {
        ret = init_p128_service(mpsoc);
    }

    if (ret == SUCCESS) {
        ret = hvc_service_append_slist(&(mpsoc->hvc_service_list), &(p128.service));
    }

    return ret;
}

