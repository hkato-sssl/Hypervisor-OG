// SPDX-License-Identifier: Apache-2.0
/*
 * exception/handler_el1.c - the default exception handler
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/exception.h"
#include "driver/aarch64/system_register.h"
#include "lib/bit.h"
#include "lib/system/printk.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

void exception_handler_el1(uint64_t *ei) __attribute__((weak));

/* variables */

void (*interrupt_handler)(uint16_t vector);

/* functions */

void exception_handler_el1(uint64_t *ei)
{
    printk("<%s>\n", __func__);
    aarch64_exception_dump(ei);
    printk("FAR_EL1: %p\n", aarch64_read_far_el1());
    for (;;)
        ;
}

void interrupt_handler_el1(uint16_t vector)
{
    if (interrupt_handler != NULL) {
        interrupt_handler(vector);
    } else {
        printk("<%s>\n", __func__);
    }
}
