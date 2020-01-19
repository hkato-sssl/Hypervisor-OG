/*
 * arm/gic400/gic400_dump_ns_cpuif.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/printk.h"
#include "driver/system/cpu.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "gic400_local.h"

/* defines */

#define ELEMENT(reg)    { #reg, reg }

/* types */

/* prototypes */

/* variables */

static const struct {
    const char  *name;
    uint32_t    reg;
} list[] = {
    ELEMENT(GICC_CTLR),
    ELEMENT(GICC_PMR),
    ELEMENT(GICC_BPR),
    ELEMENT(GICC_RPR),
    ELEMENT(GICC_HPPIR),
    ELEMENT(GICC_IIDR),
    { NULL, 0 }         /* terminator */
};

/* functions */

void gic400_dump_ns_cpuif(const struct gic400 *gic)
{
    int i;

    printk("<GIC-400 cpuif#%d>\n", cpu_no());

    for (i = 0; list[i].name != NULL; ++i) {
        printk("%-12s: %08x\n", list[i].name, gic400_read_cpuif(gic, list[i].reg));
    }
}

