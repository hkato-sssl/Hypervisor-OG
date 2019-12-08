/*
 * hypervisor/test/vm/01_launch.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

#define RAM_START   0x00000000
#define RAM_SIZE    0x20000000

#define UART_IPA    0xa0001000
#define UART_PA     0xa0001000
#define UART_SIZE   4096

/* types */

/* prototypes */

extern struct aarch64_stage2 hyp_test_stage2;

errno_t hyp_test_stage2_init(void);
void test_vm_launch_guest_start(void);

/* variables */

static struct vm vm;
static uint64_t regs[CONFIG_NR_CPUS][NR_VPC_REGS];

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

static errno_t init_vm(void)
{
    errno_t ret;
    struct vm_configuration config;

    memset(&config, 0, sizeof(config));
    config.nr_vpcs = 1;
    config.vpc[0].regs = regs[0];
    config.vpc[0].gpr.pc = (uint64_t)test_vm_launch_guest_start;
    config.vpc[0].gpr.sp = 0;
    config.stage2 = &hyp_test_stage2;
    ret = vm_configure(&vm, &config);

    return ret;
}

void test_vm_01_launch(void)
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
        ret = vpc_launch(&(vm.vpc[0]));
        printk("vpc_launch() -> %d\n", ret);
        printk("\n");
        vpc_simple_dump(&vm.vpc[0]);
        printk("VTTBR_EL2: 0x%016llx\n", aarch64_read_vttbr_el2());
        printk(" VTCR_EL2: 0x%016llx\n", aarch64_read_vtcr_el2());
        printk("\n");
        aarch64_stage2_dump_descriptor(vm.stage2);
    }

    printk("Done\n");
}

