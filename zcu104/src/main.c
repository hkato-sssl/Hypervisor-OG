/*
 * main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/memio.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/system/cpu.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/cache.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t init_system(void);
errno_t init_memory_map(void);
void init_hw(void);
void hypervisor_start(void);

static void test(void)
{
    uint32_t *p;
    uint32_t d;
    uint32_t d0;
    uintptr_t base;
    int ct;

    printk("%s()\n", __func__);

    p = (void *)0x40000000;
    *p = 0;
    aarch64_dcache_clean_invd(p);

    base = 0xa0002000;
    REG_WRITE32(base, 8, (uintptr_t)p);
    REG_WRITE32(base, 12, 0x112233ff);

    for (ct = 0;; ct++) {
        d0 = REG_READ32(base, 0x10);
        if ((d0 & BIT(4)) == 0) {
            break;
        }
    }

    printk("ct:%d\n", ct);
    printk("*p=%p\n", *p);
    printk("d0=0x%08x\n", d0);
    p = (void *)0x50000000;
    printk("*p=%p\n", *p);
}

int main(void)
{
    init_system();
    init_memory_map();
    init_hw();

    if (cpu_no() == 0) {
      test_arm_smmu500_02();
      test();
      test_arm_smmu500_02_dump();
    }

    //hypervisor_start();

    return 0;
}
