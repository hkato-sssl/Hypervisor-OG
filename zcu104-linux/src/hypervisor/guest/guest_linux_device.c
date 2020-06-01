/*
 * hypervisor/guest/guest_linux_device.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "driver/arm/smmu500.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/soc.h"

/* defines */

#define RAM_START_IPA       0x00000000  /* 512MB */
#define RAM_START_PA        0x00000000
#define RAM_SIZE            0x20000000

#define UART_IPA            0xff000000  /* PS UART0 */
#define UART_PA             0xff000000
#define UART_SIZE           4096

#define AXI_SMMU_TEST_IPA   0xa0002000
#define AXI_SMMU_TEST_PA    0xa0002000
#define AXI_SMMU_TEST_SIZE  4096

/* types */

/* prototypes */

/* variables */

static struct soc_device ram = {
    .nr_irqs = 0,
    .irqs = NULL,
    .region.ipa = RAM_START_IPA,
    .region.pa = RAM_START_PA,
    .region.size = RAM_SIZE,
    .region.memory_type = HYP_MMU_MT_NORMAL_WB,
    .region.shareability = HYP_MMU_SH_ISH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 1
};

static struct soc_device_interrupt uart0_irqs[] = { { 53, 53}  };
static struct soc_device uart0 = {
    .nr_irqs = 1,
    .irqs = uart0_irqs,
    .region.ipa = UART_IPA,
    .region.pa = UART_PA,
    .region.size = UART_SIZE,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device_interrupt rtc_irqs[] = { { 58, 58} , { 59, 59 } };
static struct soc_device rtc = {
    .nr_irqs = 2,
    .irqs = rtc_irqs,
    .region.ipa = 0xffa60000,
    .region.pa = 0xffa60000,
    .region.size = 0x1000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device ddrc = {
    .nr_irqs = 0,
    .irqs = NULL,
    .region.ipa = 0xfd070000,
    .region.pa = 0xfd070000,
    .region.size = 0x1000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 0,
    .region.access.flag.exec = 0
};

static struct soc_device_interrupt ocm_irqs[] = { { 42, 42 } };
static struct soc_device ocm = {
    .nr_irqs = 1,
    .irqs = ocm_irqs,
    .region.ipa = 0xff960000,
    .region.pa = 0xff960000,
    .region.size = 0x1000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device_interrupt gem3_irqs[] = { { 95, 95 } };
static struct soc_device gem3 = {
    .nr_irqs = 1,
    .irqs = gem3_irqs,
    .region.ipa = 0xff0e0000,
    .region.pa = 0xff0e0000,
    .region.size = 0x1000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device_interrupt sd1_irqs[] = { { 81, 81 } };
static struct soc_device sd1 = {
    .nr_irqs = 1,
    .irqs = sd1_irqs,
    .region.ipa = 0xff170000,
    .region.pa = 0xff170000,
    .region.size = 0x1000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device xhci0_reg = {
    .nr_irqs = 0,
    .irqs = NULL,
    .region.ipa = 0xff9d0000,
    .region.pa = 0xff9d0000,
    .region.size = 0x1000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device_interrupt xhci0_irqs[] = { { 97, 97 } };
static struct soc_device xhci0 = {
    .nr_irqs = 1,
    .irqs = xhci0_irqs,
    .region.ipa = 0xfe200000,
    .region.pa = 0xfe200000,
    .region.size = 0x10000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device serdes0 = {
    .nr_irqs = 0,
    .irqs = NULL,
    .region.ipa = 0xfd400000,
    .region.pa = 0xfd400000,
    .region.size = 0x20000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device axi_smmu_test = {
    .nr_irqs = 0,
    .irqs = NULL,
    .region.ipa = AXI_SMMU_TEST_IPA,
    .region.pa = AXI_SMMU_TEST_PA,
    .region.size = AXI_SMMU_TEST_SIZE,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

uint16_t nr_guest_linux_devices = 10;
struct soc_device *guest_linux_devices[] = {
    &ram,
    &uart0,
    &rtc,
    &ddrc,
    &ocm,
    &sd1,
    &xhci0_reg,
    &xhci0,
    &serdes0,
    &gem3,
    &axi_smmu_test,
};

static struct smmu_stream stream = {
    .mask = 0xffff,
    .id = 0x0e80
};

uint16_t nr_guest_linux_streams = 1;
struct smmu_stream *guest_linux_streams[] = {
    &stream
};

