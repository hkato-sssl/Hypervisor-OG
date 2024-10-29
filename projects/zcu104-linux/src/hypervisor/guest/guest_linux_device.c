// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/guest/guest_linux_device.c
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
#define RAM_START_PA  CONFIG_GUEST1_BASE
#define RAM_SIZE      CONFIG_GUEST1_SIZE

#define UART_IPA      0xff000000 /* PS UART0 */
#define UART_PA       0xff000000
#define UART_SIZE     4096

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

static struct soc_device_interrupt uart0_irqs[] = {{53, 53}};
static struct soc_device_region uart0_region = {.ipa = UART_IPA,
                                                .pa = UART_PA,
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

static struct soc_device_interrupt ocm_irqs[] = {{42, 42}};
static struct soc_device_region ocm_region = {.ipa = 0xff960000,
                                              .pa = 0xff960000,
                                              .size = 0x1000,
                                              .memory_type =
                                                  HYP_MMU_MT_DEVICE_nGnRE,
                                              .shareability = HYP_MMU_SH_OSH,
                                              .access.flag.read = 1,
                                              .access.flag.write = 1,
                                              .access.flag.exec = 0};
static struct soc_device_region *ocm_regions[] = {
    &ocm_region,
};
static struct soc_device ocm = {
    .nr_irqs = 1,
    .irqs = ocm_irqs,
    .nr_regions = 1,
    .regions = ocm_regions,
};

static struct soc_device_interrupt gem3_irqs[] = {{95, 95}};
static struct soc_device_region gem3_region = {.ipa = 0xff0e0000,
                                               .pa = 0xff0e0000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 1,
                                               .access.flag.exec = 0};
static struct soc_device_region *gem3_regions[] = {
    &gem3_region,
};
static struct soc_device gem3 = {
    .nr_irqs = 1,
    .irqs = gem3_irqs,
    .nr_regions = 1,
    .regions = gem3_regions,
};

static struct soc_device_interrupt sd1_irqs[] = {{81, 81}};
static struct soc_device_region sd1_region = {.ipa = 0xff170000,
                                              .pa = 0xff170000,
                                              .size = 0x1000,
                                              .memory_type =
                                                  HYP_MMU_MT_DEVICE_nGnRE,
                                              .shareability = HYP_MMU_SH_OSH,
                                              .access.flag.read = 1,
                                              .access.flag.write = 1,
                                              .access.flag.exec = 0};
static struct soc_device_region *sd1_regions[] = {
    &sd1_region,
};
static struct soc_device sd1 = {
    .nr_irqs = 1,
    .irqs = sd1_irqs,
    .nr_regions = 1,
    .regions = sd1_regions,
};

static struct soc_device_region serdes0_region = {
    .ipa = 0xfd400000,
    .pa = 0xfd400000,
    .size = 0x20000,
    .memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .shareability = HYP_MMU_SH_OSH,
    .access.flag.read = 1,
    .access.flag.write = 0,
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

static struct soc_device_interrupt hvcs_p128_irqs[] = {{168, 168}};
static struct soc_device hvcs_p128 = {
    .nr_irqs = 1,
    .irqs = hvcs_p128_irqs,
    .nr_regions = 0,
    .regions = NULL,
};

static struct soc_device_interrupt gpio_irqs[] = {{48, 48}};
static struct soc_device_region gpio_region = {.ipa = 0xff0a0000,
                                               .pa = 0xff0a0000,
                                               .size = 0x1000,
                                               .memory_type =
                                                   HYP_MMU_MT_DEVICE_nGnRE,
                                               .shareability = HYP_MMU_SH_OSH,
                                               .access.flag.read = 1,
                                               .access.flag.write = 1,
                                               .access.flag.exec = 0};
static struct soc_device_region *gpio_regions[] = {
    &gpio_region,
};
static struct soc_device gpio = {
    .nr_irqs = 1,
    .irqs = gpio_irqs,
    .nr_regions = 1,
    .regions = gpio_regions,
};

uint16_t nr_guest_linux_devices = 10;
struct soc_device *guest_linux_devices[] = {
    &ram, &uart0, &rtc, &ddrc, &ocm, &sd1, &serdes0, &gem3, &hvcs_p128, &gpio,
};

static struct smmu_stream tbu2_sd1 = {
    .mask = 0, .id = 0x0871 /* 00010, 0001 11 0000 */
};

static struct smmu_stream tbu2_gem3 = {
    .mask = 0, .id = 0x0877 /* 00010, 0001 11 0111 */
};

uint16_t nr_guest_linux_streams = 2;
struct smmu_stream *guest_linux_streams[] = {
    &tbu2_sd1,
    &tbu2_gem3,
};
