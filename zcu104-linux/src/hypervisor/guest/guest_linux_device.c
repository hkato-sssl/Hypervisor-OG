/*
 * hypervisor/guest/guest_linux_device.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "driver/arm/smmu500.h"
#include "hypervisor/soc.h"

/* defines */

#define RAM_START_PA        0x00000000  /* 1GB */
#define RAM_START_IPA       0x00000000
#define RAM_SIZE            0x40000000

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
    .region.pa = RAM_START_PA,
    .region.ipa = RAM_START_IPA,
    .region.size = RAM_SIZE,
    .region.memory_type = HYP_MMU_MT_NORMAL_WB,
    .region.shareability = SOC_ISH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 1
};

static uint16_t uart0_irqs[] = { 53 };
static struct soc_device uart0 = {
    .nr_irqs = 1,
    .irqs = uart0_irqs,
    .region.pa = UART_PA,
    .region.ipa = UART_IPA,
    .region.size = UART_SIZE,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = SOC_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

static struct soc_device axi_smmu_test = {
    .nr_irqs = 0,
    .irqs = NULL,
    .region.pa = AXI_SMMU_TEST_PA,
    .region.ipa = AXI_SMMU_TEST_IPA,
    .region.size = AXI_SMMU_TEST_SIZE,
    .region.memory_type = HYP_MMU_MT_DEVICE_nGnRE,
    .region.shareability = SOC_OSH,
    .region.access.flag.read = 1,
    .region.access.flag.write = 1,
    .region.access.flag.exec = 0
};

uint16_t nr_guest_linux_devices = 3;
struct soc_device *guest_linux_devices[] = {
    &ram,
    &uart0,
    &axi_smmu_test,
};

static struct smmu_stream stream = {
    .mask = 0x7fff,
    .id = 0x0e80
};

uint16_t nr_guest_linux_streams = 1;
struct smmu_stream *guest_linux_streams[] = {
    &stream
};

