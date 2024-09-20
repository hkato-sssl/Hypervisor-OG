/*
 * hypervisor/guest/guest_usb_linux.c
 *
 * (C) 2020 Hidekazu Kato
 */

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

#define NR_CPUS    4
#define GUEST_VMID 2

/* types */

/* prototypes */

static errno_t el2_irq_handler(struct vpc *vpc, struct vgic400 *vgic,
                               uint32_t iar);

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
static struct vpc vpcs[NR_CPUS];
static uint64_t regs[NR_CPUS][NR_VPC_REGS] __attribute__((aligned(32)));
static struct vpc_exception_ops ops;
static struct vgic400_ops xilinx_mpsoc_vgic400_ops = {
    .irq_handler = vgic400_default_irq_handler,
};

/* functions */

static errno_t emulate_hvc(const struct insn *insn)
{
    gic400_dump_ns_cpuif(&sys_gic);
    gic400_dump_ns_distributor(&sys_gic);
    smmu500_dump(&sys_smmu);
    smmu500_dump_stream_match_register(&sys_smmu, 0);

    return -ENOSYS;
}

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

    for (i = 0; i < NR_CPUS; ++i) {
        config.vpc.vpcs[i] = &(vpcs[i]);
        config.vpc.register_arrays[i] = regs[i];
    }

    ops = *xilinx_mpsoc_default_vpc_exception_ops();
    // ops.aarch64.hvc = emulate_hvc;
    config.vpc.ops = &ops;

    config.vmid = GUEST_VMID;
    config.nr_procs = NR_CPUS;
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
    config.gic.boolean.virtual_spi = true;
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
