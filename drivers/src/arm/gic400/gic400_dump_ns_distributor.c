/*
 * arm/gic400/gic400_dump_ns_distributor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "lib/system.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "gic400_local.h"

/* defines */

#define ELEMENT(reg)        { #reg, reg }
#define ELEMENT_A(reg, d)   { #reg, reg(0), d }

/* types */

/* prototypes */

/* variables */

static struct reg_type1 {
    const char  *name;
    uint32_t    reg;
} list1[] = {
    ELEMENT(GICD_CTLR),
    ELEMENT(GICD_TYPER),
    ELEMENT(GICD_IIDR),
    { NULL, 0 }         /* terminator */
}, list2[] = {
    ELEMENT(GICD_PIDR4),
    ELEMENT(GICD_PIDR5),
    ELEMENT(GICD_PIDR6),
    ELEMENT(GICD_PIDR7),
    ELEMENT(GICD_PIDR0),
    ELEMENT(GICD_PIDR1),
    ELEMENT(GICD_PIDR2),
    ELEMENT(GICD_PIDR3),
    ELEMENT(GICD_CIDR0),
    ELEMENT(GICD_CIDR1),
    ELEMENT(GICD_CIDR2),
    ELEMENT(GICD_CIDR3),
    { NULL, 0 }         /* terminator */
} ;

static struct reg_type2 {
    const char  *name;
    uint32_t    reg;
    uint32_t    diff;
} list3[] = {
    ELEMENT_A(GICD_ISENABLER, 32),
    ELEMENT_A(GICD_ISPENDR, 32),
    ELEMENT_A(GICD_ISACTIVER, 32),
    ELEMENT_A(GICD_IPRIORITYR, 4),
    ELEMENT_A(GICD_ITARGETSR, 4),
    ELEMENT_A(GICD_ICFGR, 16),
    { NULL, 0, 0 }      /* terminator */
};

static const char out_fmt[] = "%-17s: %08x\n";

/* functions */


static void dump_sr(const struct gic400 *gic)
{
    int n;
    uint32_t d;
    uint32_t reg;
    char buff[32];

    d = gic400_read_distributor(gic, GICD_PPISR);
    printk(out_fmt, "GICD_PPISR", d);
    n = 0;
    reg = GICD_SPISR(0);
    for (d = 32; d < gic->nr_interrupts; d += 32) {
        snprintf(buff, sizeof(buff), "GICD_SPISR%d", n);
        printk(out_fmt, buff, gic400_read_distributor(gic, reg));
        reg += 4;
        ++n;
    }
 
}

static void dump_array(const struct gic400 *gic, const struct reg_type2 list[])
{
    int i;
    int n;
    uint32_t d;
    uint32_t reg;
    char buff[32];

    for (i = 0; list[i].name != NULL; ++i) {
        reg = list[i].reg;
        n = 0;
        for (d = 0; d < gic->nr_interrupts; d += list[i].diff) {
            snprintf(buff, sizeof(buff), "%s%d", list[i].name, n);
            printk(out_fmt, buff, gic400_read_distributor(gic, reg));
            reg += 4;
            ++n;
        }
    }
}

static void dump(const struct gic400 *gic, const struct reg_type1 list[])
{
    int i;

    for (i = 0; list[i].name != NULL; ++i) {
        printk(out_fmt, list[i].name, gic400_read_distributor(gic, list[i].reg));
    }
}

static void dump_ns_distributor(const struct gic400 *gic)
{
    printk("<GIC-400 Distributor>\n");
    printk("# of interrupts: %u\n", gic->nr_interrupts);
    dump(gic, list1);
    dump_array(gic, list3);
    dump_sr(gic);
    dump(gic, list2);
}

void gic400_dump_ns_distributor(const struct gic400 *gic)
{
    if (gic != NULL) {
        dump_ns_distributor(gic);
    }
}

