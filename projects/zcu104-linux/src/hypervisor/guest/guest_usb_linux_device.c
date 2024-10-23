/*
 * hypervisor/guest/guest_usb_linux_device.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "config/system.h"

#include "driver/aarch64/stage2.h"
#include "driver/arm/smmu500.h"
#include "hypervisor/soc.h"
#include <stdint.h>
#include <string.h>

/* defines */

#define RAM_START_IPA 0x00000000 /* 512MB */
#define RAM_START_PA  CONFIG_GUEST2_BASE
#define RAM_SIZE      CONFIG_GUEST2_SIZE

/* types */

/* prototypes */

/* variables */

static struct soc_device_region ram_region = {.ipa = RAM_START_IPA,
                                              .pa = RAM_START_PA,
                                              .size = RAM_SIZE,
                                              .memory_type =
                                                  HYP_MMU_MT_NORMAL_WB,
                                              .shareability = HYP_MMU_SH_ISH,
                                              .access.flag.read = 1,
                                              .access.flag.write = 1,
                                              .access.flag.exec = 1};
static struct soc_device_region *ram_regions[] = {
    &ram_region,
};
static struct soc_device ram = {
    .nr_irqs = 0,
    .irqs = NULL,
    .nr_regions = 1,
    .regions = ram_regions,
};

static struct soc_device_interrupt uart0_irqs[] = {{53, 54}};
static struct soc_device_region uart0_region = {.ipa = 0xff000000,
                                                .pa = 0xff010000,
                                                .size = 4096,
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
    .nr_irqs = 1,
    .irqs = uart0_irqs,
    .nr_regions = 1,
    .regions = uart0_regions,
};

static struct soc_device_interrupt rtc_irqs[] = {{58, 58}, {59, 59}};
static struct soc_device_region rtc_region = {.ipa = 0xffa60000,
                                              .pa = 0xffa60000,
                                              .size = 0x1000,
                                              .memory_type =
                                                  HYP_MMU_MT_DEVICE_nGnRE,
                                              .shareability = HYP_MMU_SH_OSH,
                                              .access.flag.read = 1,
                                              .access.flag.write = 1,
                                              .access.flag.exec = 0};
static struct soc_device_region *rtc_regions[] = {
    &rtc_region,
};
static struct soc_device rtc = {
    .nr_irqs = 2,
    .irqs = rtc_irqs,
    .nr_regions = 1,
    .regions = rtc_regions,
};

static struct soc_device_region ddrc_region = {.ipa = 0xfd070000,
                                               .pa = 0xfd070000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 0,
                                               .access.flag.exec = 0};
static struct soc_device_region *ddrc_regions[] = {
    &ddrc_region,
};
static struct soc_device ddrc = {
    .nr_irqs = 0,
    .irqs = NULL,
    .nr_regions = 1,
    .regions = ddrc_regions,
};

static struct soc_device_interrupt xhci0_irqs[] = {{97, 97}};
static struct soc_device_region xhci0_reg_region = {
    .ipa = 0xff9d0000,
    .pa = 0xff9d0000,
    .size = 0x1000,
    .memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .shareability = HYP_MMU_SH_OSH,
    .access.flag.read = 1,
    .access.flag.write = 1,
    .access.flag.exec = 0};
static struct soc_device_region xhci0_region = {.ipa = 0xfe200000,
                                                .pa = 0xfe200000,
                                                .size = 0x10000,
                                                .memory_type =
                                                    HYP_MMU_MT_DEVICE_nGnRE,
                                                .shareability = HYP_MMU_SH_OSH,
                                                .access.flag.read = 1,
                                                .access.flag.write = 1,
                                                .access.flag.exec = 0};
static struct soc_device_region *xhci0_regions[] = {
    &xhci0_reg_region,
    &xhci0_region,
};
static struct soc_device xhci0 = {
    .nr_irqs = 1,
    .irqs = xhci0_irqs,
    .nr_regions = 2,
    .regions = xhci0_regions,
};

static struct soc_device_region serdes0_region = {
    .ipa = 0xfd400000,
    .pa = 0xfd400000,
    .size = 0x20000,
    .memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .shareability = HYP_MMU_SH_OSH,
    .access.flag.read = 1,
    .access.flag.write = 1,
    .access.flag.exec = 0};
static struct soc_device_region *serdes0_regions[] = {
    &serdes0_region,
};
static struct soc_device serdes0 = {
    .nr_irqs = 0,
    .irqs = NULL,
    .nr_regions = 1,
    .regions = serdes0_regions,
};

static struct soc_device_interrupt i2c1_irqs[] = {{50, 50}};
static struct soc_device_region i2c1_region = {.ipa = 0xff030000,
                                               .pa = 0xff030000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 1,
                                               .access.flag.exec = 0};
static struct soc_device_region *i2c1_regions[] = {
    &i2c1_region,
};
static struct soc_device i2c1 = {
    .nr_irqs = 1,
    .irqs = i2c1_irqs,
    .nr_regions = 1,
    .regions = i2c1_regions,
};

static struct soc_device_interrupt dp_irqs[] = {{151, 151}, {154, 154}};
static struct soc_device_region dp_region_1 = {.ipa = 0xfd4a0000,
                                               .pa = 0xfd4a0000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 1,
                                               .access.flag.exec = 0};
static struct soc_device_region dp_region_2 = {.ipa = 0xfd4aa000,
                                               .pa = 0xfd4aa000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 1,
                                               .access.flag.exec = 0};
static struct soc_device_region dp_region_3 = {.ipa = 0xfd4ab000,
                                               .pa = 0xfd4ab000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 1,
                                               .access.flag.exec = 0};
static struct soc_device_region dp_region_4 = {.ipa = 0xfd4ac000,
                                               .pa = 0xfd4ac000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 1,
                                               .access.flag.exec = 0};
static struct soc_device_region dp_dma_region = {.ipa = 0xfd4c0000,
                                                 .pa = 0xfd4c0000,
                                                 .size = 0x1000,
                                                 .memory_type =
                                                     HYP_MMU_MT_DEVICE_nGnRE,
                                                 .shareability = HYP_MMU_SH_OSH,
                                                 .access.flag.read = 1,
                                                 .access.flag.write = 1,
                                                 .access.flag.exec = 0};
static struct soc_device_region *dp_regions[] = {
    &dp_region_1, &dp_region_2, &dp_region_3, &dp_region_4, &dp_dma_region};
static struct soc_device dp = {
    .nr_irqs = 2,
    .irqs = dp_irqs,
    .nr_regions = 5,
    .regions = dp_regions,
};

static struct soc_device_interrupt hvcs_p128_irqs[] = {{170, 170}};
static struct soc_device hvcs_p128 = {
    .nr_irqs = 1,
    .irqs = hvcs_p128_irqs,
    .nr_regions = 0,
    .regions = NULL,
};

uint16_t nr_guest_usb_linux_devices = 9;
struct soc_device *guest_usb_linux_devices[] = {
    &ram, &uart0, &rtc, &ddrc, &xhci0, &serdes0, &i2c1, &dp, &hvcs_p128};

static struct smmu_stream tbu2_usb0 = {
    .mask = 0, .id = 0x0860 /* 00010, 0001 10 0000 */
};

static struct smmu_stream tbu3_dp = {
    .mask = 0x0007, .id = 0x0ce0 /* 00011, 0011 10 0xxx */
};

uint16_t nr_guest_usb_linux_streams = 2;
struct smmu_stream *guest_usb_linux_streams[] = {&tbu2_usb0, &tbu3_dp};
