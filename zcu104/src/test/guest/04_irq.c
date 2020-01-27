/*
 * hypervisor/test/guest/04_irq.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：仮想SGI割り込み処理
 *
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

#define NR_CPUS     1

#define UART_IPA    0xff000000  /* PS UART0 */
#define UART_PA     0xff000000
#define UART_SIZE   4096

/* types */

/* prototypes */

errno_t guest_02_data_abort(const struct insn *insn, void *arg);
static errno_t emulate_hvc(struct vpc *vpc);

/* variables */

extern struct gic400 sys_gic;
extern struct aarch64_mmu_block_pool sys_pool;

static DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(table);

static struct xilinx_mpsoc mpsoc;
static struct vpc vpcs[NR_CPUS];
static uint64_t regs[NR_CPUS][NR_VPC_REGS] __attribute__ ((aligned(32)));
static const struct vpc_exception_ops ops = {
    .irq = (vpc_exception_emulator_t)vgic400_emulate_irq_exception,
    .aarch64.data_abort = (vpc_exception_emulator_t)vpc_emulate_aarch64_data_abort,
    .aarch64.hvc = (vpc_exception_emulator_t)emulate_hvc,
};

/* functions */

static errno_t emulate_hvc(struct vpc *vpc)
{
    gic400_dump_ns_cpuif(&sys_gic);
    gic400_dump_ns_distributor(&sys_gic);
    for (;;);

    return SUCCESS;
}

static void map_device(void)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 0;
    attr.af = 1;
    attr.sh = STAGE2_SH_OSH;
    attr.s2ap = STAGE2_S2AP_RW;
    attr.memattr = STAGE2_MEMATTR_DEVICE_nGnRnE;
    ret = aarch64_stage2_map(&(mpsoc.soc.stage2), (void *)UART_IPA, (void *)UART_PA, UART_SIZE, &attr);
    if (ret != SUCCESS) {
        printk("aarch64_stage2_map() -> %d\n", ret);
    }
}

static void init_vpc(void)
{
    int i;
    errno_t ret;
    struct vpc *vpc;
    struct vpc_configuration config;

    memset(&config, 0, sizeof(config));
    config.vm = &(mpsoc.soc.vm);
    config.hook = NULL;
    config.exception.ops = &ops;
    config.exception.arg = &(mpsoc.vgic400);
    for (i = 0; i < NR_CPUS; ++i) {
        vpc = &(vpcs[i]);
        config.regs = regs[i];
        config.proc_no = (uint8_t)i;
        ret = vpc_initialize(vpc, &config);
        if (ret == SUCCESS) {
            ret = vm_register_vpc(&(mpsoc.soc.vm), vpc);
            if (ret != SUCCESS) {
                printk("vm_register_vpc() -> %d\n", ret);
                break;
            }
        } else {
            printk("vpc_initialize(%d) -> %d\n", i, ret);
            break;
        }
    }

    if (ret == SUCCESS) {
        map_device();
    }
}

static void init_mpsoc(void)
{
    errno_t ret;
    struct xilinx_mpsoc_configuration config;

    memset(&config, 0, sizeof(config));
    config.nr_procs = NR_CPUS;
    config.vmid = 1;
    config.stage2.pool = &sys_pool;
    config.stage2.level1_table = table;
    config.gic = &sys_gic;
    ret = xilinx_mpsoc_initialize(&mpsoc, &config);
    if (ret == SUCCESS) {
        init_vpc();
    }
}

void test_guest_04(void)
{
    init_mpsoc();

    printk("Done\n");
}

