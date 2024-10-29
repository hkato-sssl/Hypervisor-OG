// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/guest/guest_usb_linux.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "config/system.h"

#include "driver/aarch64/mmu.h"
#include "driver/arm/gic400.h"
#include "driver/arm/smmu500.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/mmu.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

static errno_t el2_irq_handler(struct vpc *vpc, struct vgic400 *vgic,
                               uint32_t iar);
errno_t guest_usb_linux_initialize_hvc(struct xilinx_mpsoc *mpsoc);

/* variables */

extern struct gic400 sys_gic;
extern struct smmu500 sys_smmu;
extern struct aarch64_mmu sys_mmu;
extern struct aarch64_mmu_block_pool sys_pool;

extern uint16_t nr_guest_usb_linux_devices;
extern struct soc_device *guest_usb_linux_devices[];
extern uint16_t nr_guest_usb_linux_streams;
extern const struct smmu_stream *guest_usb_linux_streams[];

static DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(table);

static struct xilinx_mpsoc mpsoc;
static struct vpc vpcs[CONFIG_GUEST2_NR_CPUS];
static uint64_t regs[CONFIG_GUEST2_NR_CPUS][NR_VPC_REGS] __attribute__((aligned(32)));
static struct vpc_exception_ops ops;
static struct vgic400_ops xilinx_mpsoc_vgic400_ops = {el2_irq_handler};
static struct vgic400_interrupt_event_array event_arrays[CONFIG_GUEST2_NR_CPUS];

/* functions */

static errno_t el2_irq_handler(struct vpc *vpc, struct vgic400 *vgic,
                               uint32_t iar)
{
    struct xilinx_mpsoc *chip;

    printk("<%s>\n", __func__);

    if (iar == IRQ_SMMU500) {
        chip = vpc->vm->soc->chip;
        smmu500_dump(chip->smmu.device);
    } else {
        printk("Unrecognized IRQ#%u has occured.\n", iar);
    }

    return -ENOTSUP;
}

static void *init_mpsoc(void)
{
    errno_t ret;
    uint32_t i;
    struct xilinx_mpsoc_configuration config;

    memset(&config, 0, sizeof(config));

    for (i = 0; i < CONFIG_GUEST2_NR_CPUS; ++i) {
        config.vpc.vpcs[i] = &(vpcs[i]);
        config.vpc.register_arrays[i] = regs[i];
    }

    ops = *xilinx_mpsoc_default_vpc_exception_ops();
    config.vpc.ops = &ops;

    config.vmid = CONFIG_GUEST2_VMID;
    config.nr_procs = CONFIG_GUEST2_NR_CPUS;
    config.mmu = &sys_mmu;
    config.stage2.pool = &sys_pool;
    config.stage2.level1_table = table;
    config.gic.device = &sys_gic;
    config.gic.nr_sgis = 7;
    for (i = 0; i < 7; ++i) {
        config.gic.sgis[i] = (uint16_t)i;
    }
    config.gic.nr_ppis = 2;
    config.gic.ppis[0] = 27;
    config.gic.ppis[1] = 30;
    config.gic.ops = &xilinx_mpsoc_vgic400_ops;
    config.gic.boolean.half_priority = true;
    for (i = 0; i < CONFIG_GUEST2_NR_CPUS; ++i) {
        config.gic.event_arrays[i] = &(event_arrays[i]);
    }
    config.smmu.device = &sys_smmu;
    config.smmu.nr_streams = nr_guest_usb_linux_streams;
    config.smmu.streams = guest_usb_linux_streams;
    config.smmu.flag.fault = 1;
    config.nr_devices = nr_guest_usb_linux_devices;
    config.devices = guest_usb_linux_devices;

    ret = xilinx_mpsoc_initialize(&mpsoc, &config);

    if (ret == SUCCESS) {
        ret = guest_usb_linux_initialize_hvc(&mpsoc);
    }

    return (ret == SUCCESS) ? &mpsoc : NULL;
}

void *guest_usb_linux(void)
{
    return init_mpsoc();
}
