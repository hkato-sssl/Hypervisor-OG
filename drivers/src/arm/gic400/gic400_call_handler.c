/*
 * arm/gic400/gic400_call_handler.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "gic400_local.h"

/* defines */

#define SPRIOUS_ID_1    1022
#define SPRIOUS_ID_2    1023

/* types */

/* prototypes */

/* variables */

/* functions */

intptr_t gic400_call_handler(struct gic400 *gic)
{
    intptr_t ret;
    uint32_t i;
    uint32_t iar;
    gic400_handler_t handler;

    iar = gic400_read_cpuif(gic, GICC_IAR);
    i = iar & BITS(9, 0);
    if (i < gic->nr_interrupts) {
        handler = gic->handlers[i].entry;
        if (handler != NULL) {
            ret = (*handler)(gic->handlers[i].arg, iar);
        } else {
            ret = -ENOENT;
        }
        gic400_write_cpuif(gic, GICC_EOIR, iar);
    } else if ((i == SPRIOUS_ID_1) || (i == SPRIOUS_ID_2)) {
            ret = -EPERM;
    } else {
            gic400_write_cpuif(gic, GICC_EOIR, iar);
            ret = -EINVAL;
    }

    return ret;
}

