/*
 * exception/handler_el2.c - the default exception handler
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/printk.h"
#include "driver/aarch64/exception.h"

/* defines */

/* types */

/* prototypes */

void exception_handler_el2(uint64_t *ei) __attribute__((weak));

/* variables */

/* functions */

void exception_handler_el2(uint64_t *ei)
{
    printk("<%s>\n", __func__);
    aarch64_exception_dump(ei);

    for (;;);
}

