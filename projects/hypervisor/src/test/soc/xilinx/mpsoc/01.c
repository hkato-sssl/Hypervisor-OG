/*
 * test/soc/xilinx/mpsoc/01.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：
 * VGIC-400の再実装に伴い、GIC-400レジスタへのアクセス処理が正常に
 * エミュレーションされているかを確認する。
 * 本テストでは割り込みの仮想番号と物理番号が同じ環境を想定している。
 */

#include "driver/aarch64/mmu.h"
#include "driver/arm/gic400.h"
#include "driver/arm/smmu500.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/mmu.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

#define NR_CPUS           1
#define GUEST_VMID        1

#define NR_SPIS           256
#define DUMMY_DEVICE_ADDR 0xfff00000UL
#define DUMMY_DEVICE_SIZE 4096UL

#define BOOT_ADDR         0x280000
#define DTB_ADDR          0

#define RAM_START_PA      0x00000000 /* 1GB */
#define RAM_START_IPA     0x00000000
#define RAM_SIZE          0x40000000

#define UART_IPA          0xff000000 /* PS UART0 */
#define UART_PA           0xff000000
#define UART_SIZE         4096

/* types */

/* prototypes */

/* variables */

extern struct gic400 sys_gic;
extern struct smmu500 sys_smmu;
extern struct aarch64_mmu sys_mmu;
extern struct aarch64_mmu_block_pool sys_pool;

static DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(table);

static struct xilinx_mpsoc mpsoc;
static struct vpc vpcs[NR_CPUS];
static uint64_t regs[NR_CPUS][NR_VPC_REGS] __attribute__((aligned(32)));
static struct vpc_exception_ops ops;

static struct soc_device_interrupt irqs[NR_SPIS];

static struct soc_device dummy = {
    .nr_irqs = NR_SPIS,
    .irqs = irqs,
    .nr_regions = 0,
    .regions = NULL,
};

static struct soc_device_region ram_region = {
    .pa = RAM_START_PA,
    .ipa = RAM_START_IPA,
    .size = RAM_SIZE,
    .memory_type = HYP_MMU_MT_NORMAL_WB,
    .shareability = HYP_MMU_SH_ISH,
    .access.flag.read = 1,
    .access.flag.write = 1,
    .access.flag.exec = 1,
};
static struct soc_device_region *ram_regions[] = {
    &ram_region,
};
static struct soc_device ram = {
    .nr_irqs = 0,
    .irqs = NULL,
    .nr_regions = 1,
    .regions = ram_regions,
};

static struct soc_device_region uart0_region = {.pa = UART_PA,
                                                .ipa = UART_IPA,
                                                .size = UART_SIZE,
                                                .memory_type =
                                                    HYP_MMU_MT_DEVICE_nGnRE,
                                                .shareability = HYP_MMU_SH_OSH,
                                                .access.flag.read = 1,
                                                .access.flag.write = 1,
                                                .access.flag.exec = 0};
static struct soc_device_region *uart0_regions[] = {
    &uart0_region,
};
static struct soc_device uart0 = {
    .nr_irqs = 0,
    .irqs = NULL, /* 割り込み番号はdevices[]で設定されるので省略 */
    .nr_regions = 1,
    .regions = uart0_regions,
};

static uint16_t nr_devices = 3;
static struct soc_device *devices[] = {
    &dummy,
    &ram,
    &uart0,
};

/* functions */

static errno_t emulate_hvc(const struct insn *insn)
{
    return -ENOTSUP;
}

static void init_irqs(void)
{
    int i;

    for (i = 0; i < NR_SPIS; ++i) {
        irqs[i].virtual = (uint16_t)(32 + i);
        irqs[i].physical = (uint16_t)(32 + i);
    }
}

static errno_t init_mpsoc(void)
{
    errno_t ret;
    uint32_t i;
    struct xilinx_mpsoc_configuration config;

    memset(&config, 0, sizeof(config));

    for (i = 0; i < NR_CPUS; ++i) {
        config.vpc.vpcs[i] = &(vpcs[i]);
        config.vpc.register_arrays[i] = regs[i];
    }

    ops = *xilinx_mpsoc_default_vpc_exception_ops();
    ops.aarch64.hvc = emulate_hvc;
    config.vpc.ops = &ops;

    config.vmid = GUEST_VMID;
    config.nr_procs = NR_CPUS;
    config.stage2.pool = &sys_pool;
    config.stage2.level1_table = table;
    config.gic.device = &sys_gic;
    config.gic.nr_ppis = 4;
    config.gic.ppis[0] = 31;
    config.gic.ppis[1] = 30;
    config.gic.ppis[2] = 28;
    config.gic.ppis[3] = 27;
    config.smmu.device = &sys_smmu;
    config.smmu.nr_streams = 0;
    config.smmu.streams = NULL;
    config.nr_devices = nr_devices;
    config.devices = devices;

    ret = xilinx_mpsoc_initialize(&mpsoc, &config);
    printk("xilinx_mpsoc_initialize() -> %d\n", ret);

    return ret;
}

static errno_t init(void)
{
    init_irqs();

    return init_mpsoc();
}

void launch(void)
{
    errno_t ret;
    struct vpc_boot_configuration boot;

    gic400_set_priority_mask(&sys_gic, 0xff);

    memset(&boot, 0, sizeof(boot));
    boot.arch = VPC_ARCH_AARCH64;
    boot.pc = BOOT_ADDR;
    boot.arg = DTB_ADDR;
    ret = vm_launch(&(mpsoc.soc.vm), 0, &boot);
    printk("vm_launch() -> %d\n", ret);

    vpc_dump(mpsoc.soc.vm.vpcs[0], 0);
}

void test_xilinx_mpsoc_01(void)
{
    printk("<%s>\n", __func__);

    if (init() == SUCCESS) {
        launch();
    }
}
