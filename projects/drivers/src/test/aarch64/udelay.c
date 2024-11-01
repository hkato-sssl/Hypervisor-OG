// SPDX-License-Identifier: Apache-2.0
/*
 * test/aarch64/udelay.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void test_aarch64_udelay(void)
{
    int ct;

    printk("<%s>\n", __func__);

    ct = 0;
    for (;;) {
        aarch64_udelay(1000000);
        if (++ct < 10) {
            printk("*");
        } else {
            printk("*\n");
            ct = 0;
        }
    }
}
