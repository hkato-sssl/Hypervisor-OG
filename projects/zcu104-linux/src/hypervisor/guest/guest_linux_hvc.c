// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/guest/guest_linux_hvc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "hypervisor/hvc/p128.h"
#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/thread.h"
#include "lib/slist.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

#define DEV_NAME "P128"
#define HVC_IMM  1

#define INTR_EP0 168
#define INTR_EP1 169

/* types */

/* prototypes */

/* variables */

extern struct p2p_packet_path guest_to_guest_path;
extern const struct p2p_packet_ep_ops xilinx_mpsoc_p2p_packet_ep_ops;

static struct p2p_packet_ep ep;
static struct p2p_packet_ep *p128_eps[] = {&ep};
static struct hvc_p128_service p128;

/* functions */

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
    config.nr_eps = 1;
    config.eps = p128_eps;
    ret = hvc_p128_service_initialize(&p128, &config);

    return ret;
}

static errno_t init_ep(struct xilinx_mpsoc *mpsoc, uint16_t interrupt_no)
{
    errno_t ret;
    struct p2p_packet_ep_configuration config;

    memset(&config, 0, sizeof(config));
    config.ops = &xilinx_mpsoc_p2p_packet_ep_ops;
    config.owner = mpsoc;
    config.length = 128;
    config.interrupt_no = interrupt_no;
    ret = p2p_packet_initialize_ep(&ep, &config);

    return ret;
}

static errno_t init_p2p(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;

    ret = init_ep(mpsoc, INTR_EP0);
    if (ret == SUCCESS) {
        ret = p2p_packet_connect(&guest_to_guest_path, &ep);
    }

    return ret;
}

errno_t guest_linux_initialize_hvc(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;

    ret = init_p2p(mpsoc);

    if (ret == SUCCESS) {
        ret = init_p128_service(mpsoc);
    }

    if (ret == SUCCESS) {
        ret = hvc_service_append_slist(&(mpsoc->hvc_service_list),
                                       &(p128.service));
    }

    return ret;
}
