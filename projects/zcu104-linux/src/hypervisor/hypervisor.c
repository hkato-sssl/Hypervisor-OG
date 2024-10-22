/*
 * hypervisor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "driver/arm/smmu500.h"
#include "driver/system/cpu.h"
#include "driver/xilinx/device/mpsoc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/thread.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

#include "driver/aarch64/system_register.h"

/* defines */

#define BOOT_ADDR 0x280000
#define DTB_ADDR  0x100000

/* types */

/* prototypes */

struct xilinx_mpsoc *guest_linux(void);
struct xilinx_mpsoc *guest_usb_linux(void);

errno_t guest_initialize_shared_resource(void);

/* variables */

extern struct gic400 sys_gic;

/* functions */

/* Guest Thread：
 * 各 Thread は割り当てられた Gest 環境上で、１つの仮想プロセッサを起動する。
 */
static void linux_guest_thread(struct xilinx_mpsoc *chip, uint32_t vpc_no,
                               uintptr_t boot_addr, uintptr_t dtb_addr)
{
    errno_t ret;
    struct vpc_boot_configuration boot;

    if (vpc_no == 0) {
        memset(&boot, 0, sizeof(boot));
        boot.arch = VPC_ARCH_AARCH64;
        boot.pc = boot_addr;
        boot.arg = dtb_addr;
        ret = vm_launch(&(chip->soc.vm), vpc_no, &boot);
        printk("vm_launch() -> %d\n", ret);
    } else {
        ret = vm_launch_secondary(&(chip->soc.vm), vpc_no);
        printk("vm_launch_secondary() -> %d\n", ret);
    }

    /* The following code will be executed only if an error occurs. */
    
    vpc_dump(chip->soc.vm.vpcs[vpc_no], 0);
}

static void launch_linux_thread(const char *name, int proc_no,
                                struct xilinx_mpsoc *chip, int vpc_no)
{
    struct thread_parameter parameter;

    printk("Start a thread for %s: PhysicalProcessor=%d VirtualProcessor=%d\n", name, proc_no, vpc_no);

    memset(&parameter, 0, sizeof(parameter));
    parameter.entry = (thread_entry_t)linux_guest_thread;
    parameter.args[0] = (uintptr_t)chip;
    parameter.args[1] = vpc_no;
    parameter.args[2] = BOOT_ADDR;
    parameter.args[3] = DTB_ADDR;
    parameter.args[4] = (uintptr_t)name;

    if (proc_no == aarch64_cpu_no()) {
        thread_launch_self(&parameter);
    } else {
        thread_launch(proc_no, &parameter);
    }
}

/* Hypervisor thread 起動処理：
 * Guest 上の仮想プロセッサ毎に Thread を１つ起動する。
 */
void hypervisor_main(void)
{
    struct xilinx_mpsoc *chip;

    gic400_enable_interrupt(&sys_gic, IRQ_SMMU500);
    gic400_set_priority_mask(&sys_gic, 0xff);

    guest_initialize_shared_resource();

    chip = guest_usb_linux();
    launch_linux_thread("Linux#1", 1, chip, 1);
    launch_linux_thread("Linux#1", 3, chip, 0);

    chip = guest_linux();
    launch_linux_thread("Linux#0", 2, chip, 1);
    launch_linux_thread("Linux#0", 0, chip, 0);
}

void hypervisor(void)
{
    /* Hypervisor 起動処理：
     * 本関数でシステム上で起動される最初の Thread を生成する。
     * システム 起動時は Primary Processor により本関数が最初に呼ばれる。
     * この時、全ての Secondary Processor は Standby 状態にある。
     */

    struct thread_parameter parameter;

    printk("Start a startup thread.\n");

    memset(&parameter, 0, sizeof(parameter));
    parameter.entry = (thread_entry_t)hypervisor_main;
    thread_launch_self(&parameter);
}
