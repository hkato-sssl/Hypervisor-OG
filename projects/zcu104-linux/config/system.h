// SPDX-License-Identifier: Apache-2.0
/*
 * config/system.h
 *
 * (C) 2019 Hidekazu Kato
 */

/* defines */

#define SYSTEM_VERSION              "1.0.0"

#define CONFIG_NR_CPUS              4

#define CONFIG_STACK_SIZE           (4 * 1024)
#define CONFIG_EXC_STACK_SIZE       (4 * 1024)
#define CONFIG_KERNEL_STACK_SIZE    (4 * 1024)
#define CONFIG_NR_MMU_MEMORY_BLOCKS 1024

#define CONFIG_GICD_BASE            0xf9010000
#define CONFIG_GICC_BASE            0xf9020000
#define CONFIG_GICH_BASE            0xf9040000
#define CONFIG_GICV_BASE            0xf9060000

#define CONFIG_HYP_UART_BASE        0xa0000000
#define CONFIG_HYP_UART_SIZE        0x1000

#define CONFIG_GUEST1_NR_CPUS       2
#define CONFIG_GUEST1_VMID          1
#define CONFIG_GUEST1_BASE          0x00000000
#define CONFIG_GUEST1_SIZE          0x40000000

#define CONFIG_GUEST2_NR_CPUS       2
#define CONFIG_GUEST2_VMID          2
#define CONFIG_GUEST2_BASE          0x40000000
#define CONFIG_GUEST2_SIZE          0x3f000000
