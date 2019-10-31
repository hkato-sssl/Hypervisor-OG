/*
 * test/vpc/launch.c - VPC 起動テスト
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

void test_vpc_launch_entry(void);

/* variables */

static struct vpc vpc;
static struct vpc_config config;
static uint64_t register_array[NR_VPC_REGS];

/* functions */

void test_vpc_launch(void)
{
    errno_t ret;

    printk("<%s>\n", __func__);

    memset(&config, 0, sizeof(config));

    config.owner = (void*)0xff; // dummy
    config.regs = register_array;
    config.aarch = 64;
    config.gpr.pc = (uint64_t)test_vpc_launch_entry;
    config.gpr.sp = 0;

    ret = vpc_configure(&vpc, &config);
    printk("vpc_configure() -> %d\n", ret);
    printk("VPC_SCTLR_EL1: 0x%08x\n", vpc.regs[VPC_SCTLR_EL1]);

    if (ret == SUCCESS) {
        vpc_launch(&vpc);
    }
}

