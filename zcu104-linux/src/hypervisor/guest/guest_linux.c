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

#define NR_CPUS         1
#define GUEST_VMID      1

#define UART_IPA        0xff000000  /* PS UART0 */
#define UART_PA         0xff000000
#define UART_SIZE       4096

#define SMMU_TEST_IPA   0xa0002000
#define SMMU_TEST_PA    0xa0002000
#define SMMU_TEST_SIZE  4096

#define RAM_START       0x00000000  /* 1GB */
#define RAM_SIZE        0x40000000

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
    config.smmu.nr_streams = 0;
    config.smmu.streams = NULL;
    config.ram.pa = RAM_START;
    config.ram.ipa = RAM_START;
    config.ram.size = RAM_SIZE;
    config.nr_devices = 0;
    config.devices = NULL;
    config.nr_ppis = 0;

    ret = xilinx_mpsoc_initialize(&mpsoc, &config);

    return (ret == SUCCESS) ? &mpsoc : NULL;
}

void *guest_linux(void)
{
    return init_mpsoc();
}

