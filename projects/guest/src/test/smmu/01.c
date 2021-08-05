/*
 * test/smmu/01.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "driver/aarch64/cache.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

__attribute__((aligned(32))) static uint32_t ddd = 0;

void test_smmu_01(void)
{
    uint32_t *p;
    uint32_t d;

    p = (void *)0xa0002000;
    REG_WRITE32(p, 0x08, 0x40000000);
    REG_WRITE32(p, 0x0c, 0xeeff9988);

    REG_WRITE32(p, 0x00, 0x40000000);
    d = REG_READ32(p, 0x04);

    printk("d=0x%08x\n", d);
    printk("s=0x%08x\n", REG_READ32(p, 0x10));
    REG_WRITE32(p, 0x00, (uintptr_t)&ddd);
    d = REG_READ32(p, 0x04);

    printk("d=0x%08x\n", d);
    printk("s=0x%08x\n", REG_READ32(p, 0x10));

    ddd = 0x11223344;
    memory_barrier();
    aarch64_dcache_invd(&ddd);

    REG_WRITE32(p, 0x00, (uintptr_t)&ddd);
    d = REG_READ32(p, 0x04);
    printk("d=0x%08x\n", d);
    printk("s=0x%08x\n", REG_READ32(p, 0x10));
}
