/*
 * hypervisor/guest/guest_usb_linux_device.c
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
#define RAM_START_PA        0x20000000
#define RAM_SIZE            0x20000000

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

static struct soc_device_interrupt uart0_irqs[] = { { 53, 54}  };
static struct soc_device uart0 = {
    .nr_irqs = 1,
    .irqs = uart0_irqs,
    .region.ipa = 0xff000000,
    .region.pa = 0xff010000,
    .region.size = 4096,
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

static struct soc_device_interrupt i2c1_irqs[] = { { 50, 50 } };
static struct soc_device i2c1 = {
    .nr_irqs = 1,
    .irqs = i2c1_irqs,
    .region.ipa = 0xff030000,
    .region.pa = 0xff030000,
    .region.size = 0x1000,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = HYP_MMU_SH_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

uint16_t nr_guest_usb_linux_devices = 10;
struct soc_device *guest_usb_linux_devices[] = {
    &ram,
    &uart0,
    &rtc,
    &ddrc,
    &sd1,
    &xhci0_reg,
    &xhci0,
    &serdes0,
    &gem3,
    &i2c1,
};

static struct smmu_stream tbu2_usb0 = {
    .mask = 0,
    .id =0x0860     /* 00010, 0001 10 0000 */
};

uint16_t nr_guest_usb_linux_streams = 1;
struct smmu_stream *guest_usb_linux_streams[] = {
    &tbu2_usb0
};
