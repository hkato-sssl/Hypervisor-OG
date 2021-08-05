/*
 * hypervisor/guest/guest_usb_linux_hvc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "hypervisor/hvc/p128.h"
#include "hypervisor/service/p2p_packet.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "lib/slist.h"
#include "lib/system/errno.h"
#include <stdint.h>
#include <string.h>

/* defines */

#define DEV_NAME "P128"
#define HVC_IMM  1

/* types */

/* prototypes */

/* variables */

static struct p2p_packet_path p2p_path;
extern struct p2p_packet_path p2p_os_path;
extern const struct p2p_packet_ep_ops xilinx_mpsoc_p2p_packet_ep_ops;

static struct p2p_packet_ep eps[3];
static struct p2p_packet_ep *p128_eps[3] = {
    &(eps[0]),
    &(eps[1]),
    &(eps[2]),
};
static struct hvc_p128_service p128;

/* functions */

static errno_t init_ep(int no, struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;
    uint16_t irq;
    struct p2p_packet_ep_configuration config;
    static char name[] = "P128#x";

    name[5] = '0' + (char)no;
    ret = vgic400_allocate_virtual_spi(&(mpsoc->vgic400), &irq, name);
    if (ret == SUCCESS) {
        memset(&config, 0, sizeof(config));
        config.ops = &xilinx_mpsoc_p2p_packet_ep_ops;
        config.owner = mpsoc;
        config.length = 128;
        config.interrupt_no = irq;
        ret = p2p_packet_initialize_ep(&(eps[no]), &config);
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
    config.nr_eps = 3;
    config.eps = p128_eps;
    ret = hvc_p128_service_initialize(&p128, &config);

    return ret;
}

static errno_t init_p2p_eps(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;

    ret = init_ep(0, mpsoc);
    if (ret == SUCCESS) {
        ret = p2p_packet_connect(&p2p_path, &(eps[0]));
    }

    if (ret == SUCCESS) {
        ret = init_ep(1, mpsoc);
        if (ret == SUCCESS) {
            ret = p2p_packet_connect(&p2p_path, &(eps[1]));
        }
    }

    if (ret == SUCCESS) {
        ret = init_ep(2, mpsoc);
        if (ret == SUCCESS) {
            ret = p2p_packet_connect(&p2p_os_path, &(eps[2]));
        }
    }

    return ret;
}

errno_t guest_usb_linux_initialize_hvc(struct xilinx_mpsoc *mpsoc)
{
    errno_t ret;

    ret = init_p2p_eps(mpsoc);

    if (ret == SUCCESS) {
        ret = init_p128_service(mpsoc);
    }

    if (ret == SUCCESS) {
        ret = hvc_service_append_slist(&(mpsoc->hvc_service_list),
                                       &(p128.service));
    }

    return ret;
}
