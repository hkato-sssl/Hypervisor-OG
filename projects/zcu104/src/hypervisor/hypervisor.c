// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "driver/system/cpu.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void test(void)
{
    void *test_guest_04(void);

    errno_t ret;
    struct xilinx_mpsoc *chip;
    struct vpc_boot_configuration boot;

    chip = test_guest_04();
    memset(&boot, 0, sizeof(boot));
    boot.arch = VPC_ARCH_AARCH64;
    boot.pc = 0x20000000;
    boot.arg = 0;
    ret = vm_launch(&(chip->soc.vm), 0, &boot);
    printk("vm_launch() -> %d\n", ret);
}

void hypervisor(void)
{
    printk("CPU#%u\n", cpu_no());

    if (cpu_no() == 3) {
        test();
    }

    for (;;);
}

