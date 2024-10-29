// SPDX-License-Identifier: Apache-2.0
/*
 * config/system.h
 *
 * (C) 2019 Hidekazu Kato
 */

/* defines */

#define CONFIG_NR_CPUS              4

#define CONFIG_STACK_SIZE           (4 * 1024)
#define CONFIG_EXC_STACK_SIZE       (4 * 1024)
#define CONFIG_KERNEL_STACK_SIZE    (4 * 1024)
#define CONFIG_NR_MMU_MEMORY_BLOCKS 1024

#define CONFIG_GICD_BASE            0xf9010000
#define CONFIG_GICC_BASE            0xf9020000
#define CONFIG_GICH_BASE            0xf9040000
#define CONFIG_GICV_BASE            0xf9060000
