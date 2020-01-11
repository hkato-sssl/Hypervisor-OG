/*
 * hypervisor/test/guest/02_insn.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

#define NR_CPUS     1

#define RAM_START   0x20000000
#define RAM_SIZE    0x01000000

#define UART_IPA    0xff000000  /* PS UART0 */
#define UART_PA     0xff000000
#define UART_SIZE   4096

#define TRAP_START  0xb0000000
#define TRAP_SIZE   0x01000000

/* types */

/* prototypes */

extern struct aarch64_stage2 hyp_test_stage2;

errno_t hyp_test_stage2_init(void);
errno_t hypervisor_init_vgic400(struct vm *vm);
errno_t guest_02_data_abort(const struct insn *insn, void *arg);

/* variables */

static struct vm vm;
static struct vpc vpcs[NR_CPUS];
static uint64_t regs[NR_CPUS][NR_VPC_REGS] __attribute__ ((aligned(32)));
static struct vm_region_trap trap;
static struct vpc_exception_ops ops = {
    .aarch64.data_abort = vpc_emulate_aarch64_data_abort,
};

/* functions */

static errno_t init_stage2_mapping(void)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 0;
    attr.af = 1;
    attr.sh = STAGE2_SH_ISH;
    attr.s2ap = STAGE2_S2AP_RW;
    attr.memattr = STAGE2_MEMATTR_NORMAL_WB;
    ret = aarch64_stage2_map(&hyp_test_stage2, (void *)RAM_START, (void *)RAM_START, RAM_SIZE, &attr);

    if (ret == SUCCESS) {
        attr.xn = 1;
        attr.af = 1;
        attr.sh = STAGE2_SH_OSH;
        attr.s2ap = STAGE2_S2AP_RW;
        attr.memattr = STAGE2_MEMATTR_DEVICE_nGnRnE;
        ret = aarch64_stage2_map(&hyp_test_stage2, (void *)UART_IPA, (void *)UART_PA, UART_SIZE, &attr);
    }

    return ret;
}

static errno_t init_trap(void)
{
    errno_t ret;

    memset(&trap, 0, sizeof(trap));
    trap.ipa.addr = TRAP_START;
    trap.ipa.size = TRAP_SIZE;
    trap.emulator.arg = NULL;
    trap.emulator.handler = guest_02_data_abort;
    ret = vm_register_region_trap(&vm, &trap);

    return ret;
}

static errno_t init_vm(void)
{
    errno_t ret;
    struct vm_configuration config;

    memset(&config, 0, sizeof(config));
    config.nr_procs = NR_CPUS;
    config.vpcs.addr = vpcs;
    config.vpcs.size = sizeof(vpcs);
    config.regs.addr = regs[0];
    config.regs.size = sizeof(regs);
    config.stage2 = &hyp_test_stage2;
    config.boot.arch = VPC_ARCH_AARCH64;
    config.boot.pc = (uint64_t)RAM_START;
    config.boot.sp = 0;
    config.vpc.exception.ops = &ops;
    ret = vm_configure(&vm, &config);
    printk("vm_configure() -> %d\n", ret);

    return ret;
}

void test_guest_02(void)
{
    errno_t ret;

    printk("<%s>\n", __func__);

    ret = hyp_test_stage2_init();
    printk("hyp_test_stage2_init() -> %d\n", ret);

    if (ret == SUCCESS) {
        ret = init_stage2_mapping();
        printk("init_stage2_mapping() -> %d\n", ret);
    }

    if (ret == SUCCESS) {
        ret = init_vm();
        printk("init_vm() -> %d\n", ret);
    }

    if (ret == SUCCESS) {
        ret = init_trap();
        printk("init_trap() -> %d\n", ret);
    }

    if (ret == SUCCESS) {
        ret = hypervisor_init_vgic400(&vm);
        printk("hypervisor_init_vgic400() -> %d\n", ret);
    }

    printk("\n\n");

    if (ret == SUCCESS) {
        ret = vm_launch(&vm);
        printk("vm_launch() -> %d\n", ret);
        printk("\n");
        vpc_dump(vm_vpc(&vm, 0), 0);
        aarch64_stage2_dump_descriptor(vm.stage2);
    }

    printk("Done\n");
}

