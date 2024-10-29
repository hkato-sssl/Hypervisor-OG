// SPDX-License-Identifier: Apache-2.0
/*
 * exception/handler_el2.c - the default exception handler
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64.h"
#include "driver/aarch64/exception.h"
#include "lib/bit.h"
#include "lib/system/printk.h"
#include "lib/system/spin_lock.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

void exception_handler_el2(uint64_t *ei) __attribute__((weak));

/* variables */

static spin_lock_t lock;

/* functions */

void exception_handler_el2(uint64_t *ei)
{
    uint32_t i;
    uint64_t *sp;

    spin_lock(&lock);

    printk("<%s#%u>\n", __func__, aarch64_cpu_no());
    aarch64_exception_dump(ei);
    printk("\n");

    sp = (uint64_t *)(ei[EXC_SP]);
    for (i = 0; i < 64; i += 2) {
        printk("%04x: %016llx %016llx\n", (i * 8), sp[i], sp[i + 1]);
    };

    printk("\n");

    spin_unlock(&lock);

    for (;;)
        ;
}
