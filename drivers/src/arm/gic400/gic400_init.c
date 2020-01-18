/*
 * arm/gic400_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/spin_lock.h"
#include "driver/system/cpu.h"
#include "driver/arm/gic400.h"
#include "driver/arm/device/gic400.h"
#include "gic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void probe_max_priority(struct gic400 *gic)
{
    uint8_t ct;
    uint8_t d;

    gic400_write_cpuif(gic, GICC_PMR, 0xff);
    d = (uint8_t)gic400_read_cpuif(gic, GICC_PMR);

    ct = 0;
    while ((d & BIT(0)) == 0) {
        ++ct;
        d >>= 1;
    }

    gic->priority.max = d;
    gic->priority.shift_ct = ct;
}

static void init_cpu_interface(struct gic400 *gic)
{
    uint32_t d;

    probe_max_priority(gic);

    /* initialize registers */

    gic400_write_cpuif(gic, GICC_PMR, 0);
    gic400_write_cpuif(gic, GICC_BPR, 0);
    if (gic->config.boolean.priority_drop) {
        d = BIT(9) | BIT(0);
    } else {
        d = BIT(0);
    }
    gic400_write_cpuif(gic, GICC_CTLR, d);
}

static void init_distributor(struct gic400 *gic)
{
    uint32_t i;
    uint32_t d;

    d = gic400_read_distributor(gic, GICD_TYPER);
    d = ((d & BITS(4, 0)) + 1) * 32;
    gic->nr_interrupts = (uint16_t)d;

    for (i = 0; i < (gic->nr_interrupts / 32); ++i) {
        gic400_write_distributor(gic, GICD_ICENABLER(i), ~(uint32_t)0);
        gic400_write_distributor(gic, GICD_ICPENDR(i), ~(uint32_t)0);
        gic400_write_distributor(gic, GICD_ICACTIVER(i), ~(uint32_t)0);
        gic400_write_distributor(gic, GICD_IGROUPR(i), 0);
    }

    for (i = 0; i < (gic->nr_interrupts / 4); ++i) {
        gic400_write_distributor(gic, GICD_IPRIORITYR(i), ~(uint32_t)0);
    }

    gic400_write_distributor(gic, GICD_CTLR, BITS(1, 0));
}

static void init_banked_distributor(struct gic400 *gic)
{
    gic400_write_distributor(gic, GICD_ICENABLER(0), ~(uint32_t)0);
    gic400_write_distributor(gic, GICD_ICPENDR(0), ~(uint32_t)0);
    gic400_write_distributor(gic, GICD_ICACTIVER(0), ~(uint32_t)0);
    gic400_write_distributor(gic, GICD_IPRIORITYR(0), ~(uint32_t)0);
    gic400_write_distributor(gic, GICD_IGROUPR(0), 0);
}

static errno_t init(struct gic400 *gic, const struct gic400_configuration *config)
{
    uint32_t lock;

    lock = cpu_lock_interrupts();

    /* initialize software parameters */

    if (cpu_no() == 0) {
        memset(gic, 0, sizeof(*gic));
        gic->config = *config;
        spin_lock_init(&(gic->lock));
    }

    /* initialize devices */
    init_cpu_interface(gic);

    if (cpu_no() == 0) {
        init_distributor(gic);
    } else {
        init_banked_distributor(gic);
    }

    cpu_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t validate_parameters(struct gic400 *gic, const struct gic400_configuration *config)
{
    errno_t ret;

    if (gic != NULL) {
        if (cpu_no() == 0) {
            /* conditions for a primary processor */
            if ((config != NULL) && (config->base.distributor != NULL) && (config->base.cpuif != NULL)) {
                ret = SUCCESS;
            } else {
                ret = -EINVAL;
            }
        } else if (config == NULL) {
            /* conditions for secondary processors */
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t gic400_init(struct gic400 *gic, const struct gic400_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(gic, config);
    if (ret == SUCCESS) {
        ret = init(gic, config);
    }

    return ret;
}
