/*
 * hypervisor.c
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
#include "hypervisor/thread.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

#include "driver/aarch64/system_register.h"

/* defines */

#define BOOT_ADDR       0x280000
#define DTB_ADDR        0x100000

/* types */

/* prototypes */

struct xilinx_mpsoc *guest_linux(void);
struct xilinx_mpsoc *guest_usb_linux(void);

/* variables */

extern struct gic400 sys_gic;

/* functions */

static void launch_guest(struct xilinx_mpsoc *chip)
{
    errno_t ret;
    struct vpc_boot_configuration boot;

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
    struct xilinx_mpsoc *chip;
    struct thread_parameter parameter;

    printk("CPU#%u\n", cpu_no());

    gic400_enable_interrupt(&sys_gic, IRQ_SMMU500);
    gic400_set_priority_mask(&sys_gic, 0xff);

    memset(&parameter, 0, sizeof(parameter));

    chip = guest_linux();
    parameter.entry = (thread_entry_t)launch_guest;
    parameter.args[0] = (uintptr_t)chip;
    thread_launch(1, &parameter);

    chip = guest_usb_linux();
    parameter.args[0] = (uintptr_t)chip;
    thread_launch(3, &parameter);

    thread_terminate();
}

void hypervisor(void)
{
    /* NOTE:
     * Hypervisor 起動時は Primary Processor により本関数が最初に呼ばれる。
     * この時、全ての Secondary Processor は Standby 状態にある。
     * 本関数実行時は thread としては起動していないので TLS へアクセスする
     * API は実行不可能。
     * 本関数の役割は H/W に関する初期化処理や thread の生成となる。
     */ 

    struct thread_parameter parameter;

    printk("<%s>\n", __func__);
    memset(&parameter, 0, sizeof(parameter));
    parameter.entry = (thread_entry_t)hypervisor_main;
    thread_launch_self(&parameter);
}
