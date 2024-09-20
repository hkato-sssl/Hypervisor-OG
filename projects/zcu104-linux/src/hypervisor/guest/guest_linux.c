/*
 * hypervisor/guest/guest_linux.c
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

#define NR_CPUS    2
#define GUEST_VMID 1

/* types */

/* prototypes */

static errno_t el2_irq_handler(struct vpc *vpc, struct vgic400 *vgic,
                               uint32_t iar);
errno_t guest_linux_initialize_hvc(struct xilinx_mpsoc *mpsoc);

/* variables */

extern struct gic400 sys_gic;
extern struct smmu500 sys_smmu;
extern struct aarch64_mmu sys_mmu;
extern struct aarch64_mmu_block_pool sys_pool;

extern uint16_t nr_guest_linux_devices;
extern struct soc_device *guest_linux_devices[];
extern uint16_t nr_guest_linux_streams;
extern const struct smmu_stream *guest_linux_streams[];

static DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(table);

static struct xilinx_mpsoc mpsoc;
static struct vpc vpcs[NR_CPUS];
static uint64_t regs[NR_CPUS][NR_VPC_REGS] __attribute__((aligned(32)));
static struct vpc_exception_ops ops;
static struct vgic400_ops xilinx_mpsoc_vgic400_ops = {
    .irq_handler = vgic400_default_irq_handler,
};
static struct vgic400_interrupt_event_array event_arrays[NR_CPUS];

/* functions */

static errno_t emulate_hvc(const struct insn *insn)
{
    struct xilinx_mpsoc *mpsoc = insn->vpc->vm->soc->chip;

    gic400_dump_ns_cpuif(&sys_gic);
    gic400_dump_ns_distributor(&sys_gic);
    gic400_dump_virtif_control(&(mpsoc->vgic400));
    // smmu500_dump(&sys_smmu);
    // smmu500_dump_stream_match_register(&sys_smmu, 0);

    return SUCCESS;
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
    for (i = 0; i < NR_CPUS; ++i) {
        config.gic.event_arrays[i] = &(event_arrays[i]);
    }
    config.smmu.device = &sys_smmu;
    config.smmu.nr_streams = nr_guest_linux_streams;
    config.smmu.streams = guest_linux_streams;
    config.smmu.flag.fault = 1;
    config.nr_devices = nr_guest_linux_devices;
    config.devices = guest_linux_devices;

    ret = xilinx_mpsoc_initialize(&mpsoc, &config);

    if (ret == SUCCESS) {
        ret = guest_linux_initialize_hvc(&mpsoc);
    }

    return (ret == SUCCESS) ? &mpsoc : NULL;
}

static void test(void);

void *guest_linux(void)
{
    void *p = init_mpsoc();
    test();
    return p;
}

#include "hypervisor/emulator/vgic400.h"

static void pr(const struct vgic400_interrupt_event *event)
{
    printk("priority: 0x%02x\n", event->priority);
    printk("src_proc: 0x%02x\n", event->src_proc);
    printk("     irq: 0x%04x\n", event->irq);
    printk("\n");
}

static void push(const struct vgic400_interrupt_event *event)
{
    struct vgic400 *vgic = &mpsoc.vgic400;
    errno_t ret = vgic400_push_interrupt_event(vgic, 0, event);
    printk("vgic400_push_interrupt_event() -> %d\n", ret);
}

static void pop(void)
{
    struct vgic400 *vgic = &mpsoc.vgic400;
    struct vgic400_interrupt_event event;
    memset(&event, 0xff, sizeof(event));
    errno_t ret = vgic400_pop_interrupt_event(vgic, &event, 0);
    printk("vgic400_pop_interrupt_event() -> %d\n", ret);
    if (ret == SUCCESS) {
        pr(&event);
    }
}

static void test(void)
{
    struct vgic400_interrupt_event event;

    memset(&event, 0, sizeof(event));
    event.priority = 0x80;
    event.irq = 48;

    push(&event);
    event.irq = 32;
    push(&event);
    event.priority = 1;
    push(&event);
    pop();
    pop();
    pop();

    for (;;)
        ;
}