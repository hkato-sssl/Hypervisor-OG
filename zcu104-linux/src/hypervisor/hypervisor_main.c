/*
 * hypervisor_main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "driver/system/cpu.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

#define BOOT_ADDR       0x28000000

/* types */

/* prototypes */

/* variables */

/* functions */

static void launch_guest(void)
{
    void *linux_guest(void);

    errno_t ret;
    struct xilinx_mpsoc *chip;
    struct vpc_boot_configuration boot;

    chip = linux_guest();
    memset(&boot, 0, sizeof(boot));
    boot.arch = VPC_ARCH_AARCH64;
    boot.pc = BOOT_ADDR;
    boot.arg = 0;
    ret = vm_launch(&(chip->soc.vm), 0, &boot);
    printk("vm_launch() -> %d\n", ret);
    vpc_dump(chip->soc.vm.vpcs[0], 0);
    
}

void hypervisor_main(void)
{
    printk("CPU#%u\n", cpu_no());

    if (cpu_no() == 0) {
        launch_guest();
    }

    for (;;);
}

