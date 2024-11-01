// SPDX-License-Identifier: Apache-2.0
/*
 * arm/gic400_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/system/cpu.h"
#include "gic400_local.h"
#include "lib/aarch64.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/spin_lock.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void probe_max_priority(struct gic400 *gic)
{
    uint8_t ct;
    uint64_t d;

    gic400_write_cpuif(gic, GICC_PMR, 0xff);
    d = gic400_read_cpuif(gic, GICC_PMR);
    d = aarch64_rbit(d);
    ct = (uint8_t)aarch64_clz(d);

    gic->priority.max = 0xff >> ct;
    gic->priority.shift_ct = ct;
}

static void probe_cpu_interface(struct gic400 *gic)
{
    probe_max_priority(gic);
}

static void initialize_cpu_interface(struct gic400 *gic)
{
    uint32_t d;

    /* initialize registers */

    gic400_write_cpuif(gic, GICC_PMR, 0);
    gic400_write_cpuif(gic, GICC_BPR, 0);
    if (gic->configuration.boolean.priority_drop) {
        d = BIT(9) | BIT(0);
    } else {
        d = BIT(0);
    }
    gic400_write_cpuif(gic, GICC_CTLR, d);
}

static void initialize_distributor(struct gic400 *gic)
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

static void initialize_banked_distributor(struct gic400 *gic)
{
    uint32_t i;

    gic400_write_distributor(gic, GICD_ICENABLER(0), ~(uint32_t)0);
    gic400_write_distributor(gic, GICD_ICPENDR(0), ~(uint32_t)0);
    gic400_write_distributor(gic, GICD_ICACTIVER(0), ~(uint32_t)0);
    gic400_write_distributor(gic, GICD_IGROUPR(0), 0);
    for (i = 0; i < 8; ++i) {
        gic400_write_distributor(gic, GICD_IPRIORITYR(i), ~(uint32_t)0);
    }
}

static errno_t initialize(struct gic400 *gic,
                          const struct gic400_configuration *config)
{
    uint32_t lock;

    lock = cpu_lock_interrupts();

    /* initialize software parameters */

    if (cpu_no() == 0) {
        memset(gic, 0, sizeof(*gic));
        gic->configuration = *config;
        spin_lock_init(&(gic->lock));
        probe_cpu_interface(gic);
    }

    /* initialize devices */
    initialize_cpu_interface(gic);

    if (cpu_no() == 0) {
        initialize_distributor(gic);
    } else {
        initialize_banked_distributor(gic);
    }

    cpu_unlock_interrupts(lock);

    return SUCCESS;
}

static errno_t validate_parameters(struct gic400 *gic,
                                   const struct gic400_configuration *config)
{
    errno_t ret;

    if (gic != NULL) {
        if (cpu_no() == 0) {
            /* conditions for a primary processor */
            if ((config != NULL) && (config->base.distributor != NULL)
                && (config->base.cpuif != NULL)) {
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

errno_t gic400_initialize(struct gic400 *gic,
                          const struct gic400_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(gic, config);
    if (ret == SUCCESS) {
        ret = initialize(gic, config);
    }

    return ret;
}
