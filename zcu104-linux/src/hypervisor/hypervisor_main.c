/*
 * hypervisor_main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/arm/smmu500.h"
#include "driver/system/cpu.h"
#include "driver/xilinx/device/mpsoc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

#include "driver/aarch64/system_register.h"

/* defines */

#define BOOT_ADDR       0x280000
#define DTB_ADDR        0x100000

/* types */

/* prototypes */

/* variables */

extern struct gic400 sys_gic;

/* functions */

static void launch_guest(void)
{
    void *guest_linux(void);

    errno_t ret;
    struct xilinx_mpsoc *chip;
    struct vpc_boot_configuration boot;

    chip = guest_linux();
    printk("guest_linux() -> %p\n", chip);

    gic400_enable_interrupt(&sys_gic, IRQ_SMMU500);
    gic400_set_priority_mask(&sys_gic, 0xff);

    memset(&boot, 0, sizeof(boot));
    boot.arch = VPC_ARCH_AARCH64;
    boot.pc = BOOT_ADDR;
    boot.arg = DTB_ADDR;
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

