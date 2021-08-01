/*
 * test/timer/test_timer_01.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/cntv_ctl_el0.h"
#include "driver/arm/gic400.h"

/* defines */

#define CYC_RATE    1000    /* unit: ms */

/* types */

/* prototypes */

/* variables */

extern struct gic400 gic;
extern void (*interrupt_handler)(uint16_t vector);

static uint32_t cyc;

/* functions */

static void update_cyc_timer(void);

static void handler(uint16_t vector)
{
    uint32_t iar;

    iar = gic400_ack(&gic);

    update_cyc_timer();

    gic400_eoi(&gic, iar);
}

static void init_interrupt(void)
{
    struct gic400_interrupt_configuration config;

    interrupt_handler = handler;

    memset(&config, 0, sizeof(config));
    config.targets = 1;
    config.priority = 0xe;
    config.flag.group1 = 0;
    config.flag.edge = 1;
    gic400_configure_interrupt(&gic, 27, &config);

    gic400_enable_interrupt(&gic, 27);
    gic400_dump_ns_distributor(&gic);
    gic400_set_priority_mask(&gic, 0xff);
}

static void init_timer(void)
{
    uint64_t d;

    aarch64_write_cntv_ctl_el0(CNTV_CTL_EL0_IMASK);
    aarch64_isb();

    d = aarch64_read_cntfrq_el0();
    d = (d * CYC_RATE) / 1000;
    cyc = (uint32_t)d;
}

static void update_cyc_timer(void)
{
    static uint64_t d;

    d = aarch64_read_cntv_cval_el0();
    d += cyc;
    aarch64_write_cntv_cval_el0(d);
    aarch64_isb();
}

static void start_timer(void)
{
    uint64_t d0, d1;

    aarch64_isb();
    d0 = aarch64_read_cntv_cval_el0();
    aarch64_isb();

    aarch64_write_cntv_tval_el0(cyc);
    aarch64_isb();

    aarch64_write_cntv_ctl_el0(CNTV_CTL_EL0_ENABLE);
    aarch64_isb();

    d1 = aarch64_read_cntv_cval_el0();
    printk("%llx\n%llx\n", d0, d1);
}

void test_timer_01(void)
{
    printk("<%s>\n", __func__);
    init_interrupt();
    init_timer();
    start_timer();
    aarch64_enable_irq();
    for (;;) {
        __asm volatile ("wfi");
        printk("*");
    }
}

