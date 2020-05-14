/*
 * hypervisor/guest/guest_linux.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/arm/smmu500.h"
#include "driver/aarch64/mmu.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/mmu.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

#define NR_CPUS             1
#define GUEST_VMID          1

#define UART_IPA            0xff000000  /* PS UART0 */
#define UART_PA             0xff000000
#define UART_SIZE           4096

#define AXI_SMMU_TEST_IPA   0xa0002000
#define AXI_SMMU_TEST_PA    0xa0002000
#define AXI_SMMU_TEST_SIZE  4096

#define RAM_START           0x00000000  /* 1GB */
#define RAM_SIZE            0x40000000

/* types */

/* prototypes */

/* variables */

extern struct gic400 sys_gic;
extern struct smmu500 sys_smmu;
extern struct aarch64_mmu sys_mmu;
extern struct aarch64_mmu_block_pool sys_pool;

static DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(table);

static struct xilinx_mpsoc mpsoc;
static struct vpc vpcs[NR_CPUS];
static uint64_t regs[NR_CPUS][NR_VPC_REGS] __attribute__ ((aligned(32)));
static struct vpc_exception_ops ops;

static uint16_t uart0_irqs[] = { 53 };
static struct soc_device uart0 = {
    .nr_irqs = 1,
    .irqs = uart0_irqs,
    .region.pa = UART_PA,
    .region.ipa = UART_IPA,
    .region.size = UART_SIZE,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device axi_smmu_test = {
    .nr_irqs = 0,
    .irqs = NULL,
    .region.pa = AXI_SMMU_TEST_PA,
    .region.ipa = AXI_SMMU_TEST_IPA,
    .region.size = AXI_SMMU_TEST_SIZE,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device *devices[] = {
    &uart0,
    &axi_smmu_test,
};

static struct smmu_stream stream = {
    .mask = 0,
    .id = 0x0e80
};

static struct smmu_stream *streams[] = {
    &stream
};

/* functions */

static errno_t emulate_hvc(struct vpc *vpc)
{
    gic400_dump_ns_cpuif(&sys_gic);
    gic400_dump_ns_distributor(&sys_gic);
    smmu500_dump(&sys_smmu);
    smmu500_dump_stream_match_register(&sys_smmu, 0);

    return -ENOSYS;
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

    config.vmid = GUEST_VMID;
    config.nr_procs = 1;
    config.stage2.pool = &sys_pool;
    config.stage2.level1_table = table;
    config.gic = &sys_gic;
    config.smmu.device = &sys_smmu;
    config.smmu.nr_streams = 1;
    config.smmu.streams = streams;
    config.smmu.flag.fault = 1;
    config.ram.pa = RAM_START;
    config.ram.ipa = RAM_START;
    config.ram.size = RAM_SIZE;
    config.nr_devices = 2;
    config.devices = devices;
    config.nr_ppis = 0;

    ret = xilinx_mpsoc_initialize(&mpsoc, &config);

    return (ret == SUCCESS) ? &mpsoc : NULL;
}

void *guest_linux(void)
{
printk("%p\n", devices);
    return init_mpsoc();
}

