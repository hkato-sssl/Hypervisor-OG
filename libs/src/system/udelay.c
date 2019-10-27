/*
 * system/udelay.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system.h"
#include "driver/aarch64.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void udelay(uint32_t usec)
{
    aarch64_udelay(usec);
}

