/*
 * config/system.h
 *
 * (C) 2019 Hidekazu Kato
 */

/* defines */

#define CONFIG_NR_CPUS                  4
#define CONFIG_STACK_SIZE               (8 * 1024)
#define CONFIG_EXC_STACK_SIZE           (8 * 1024)
#define CONFIG_NR_MMU_MEMORY_BLOCKS     1024

#define CONFIG_GICD_BASE                0xf9010000
#define CONFIG_GICC_BASE                0xf9020000

#define CONFIG_REGION_TRAP              0xb00000000000
#define CONFIG_REGION_TRAP_PA           0xa00000000
#define CONFIG_REGION_TRAP_SIZE         0x100000000
