/*
 * test/vpc/launch.c - VPC 起動テスト
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

void test_vpc_launch_entry(void);

/* variables */

static struct vpc vpc;
static uint64_t register_array[NR_VPC_REGS];

/* functions */

void test_vpc_launch(void)
{
    errno_t ret;
    struct vpc_configuration config;
    struct vpc_boot_configuration boot;

    printk("<%s>\n", __func__);

    memset(&config, 0, sizeof(config));
    config.vm = (void *)0xff; // dummy
    config.regs = register_array;
    config.proc_no = 0;

    ret = vpc_configure(&vpc, &config);
    printk("vpc_configure() -> %d\n", ret);
    printk("VPC_SCTLR_EL1: 0x%08x\n", vpc.regs[VPC_SCTLR_EL1]);

    if (ret == SUCCESS) {
        memset(&boot, 0, sizeof(boot));
        boot.arch = VPC_ARCH_AARCH64;
        boot.pc = (uint64_t)test_vpc_launch_entry;
        boot.sp = 0;
        vpc_launch(&vpc, &boot);
    }
}
