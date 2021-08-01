/*
 * aarch64/test/gt/0.c -- test of Generic Timer
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "driver/test/aarch64/gt.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void test_gt_0(void)
{
    int i;
    uintptr_t d0;
    uintptr_t d;

    printk("<%s>\n", __func__);

    d = aarch64_read_cntfrq_el0();
    printk("CNTFRQ_EL0: %llu\n", d);    

    aarch64_isb();
    d0 = aarch64_read_cntpct_el0();
    printk("CNTPCT_EL0: %llu\n", d0);   

    for (i = 0; i < 10; ++i) {
        aarch64_isb();
        d = aarch64_read_cntpct_el0();
        printk("#%d: %llu\n", i, d - d0);
        d0 = d;
    }
}
