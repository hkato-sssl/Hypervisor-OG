// SPDX-License-Identifier: Apache-2.0
/*
 * arm/gic400/gic400.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/system/cpu.h"
#include "gic400_local.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/spin_lock.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_valid_interrupt_no(const struct gic400 *gic, uint16_t intr_no)
{
    bool ret;

    if (intr_no < gic->nr_interrupts) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static void sync_dist_io(struct gic400 *gic)
{
    memory_barrier();
    gic400_read_distributor(gic, GICD_CTLR);
}

static errno_t write_dist_bit(struct gic400 *gic, uint16_t bit_no,
                              uintptr_t reg0)
{
    uint32_t reg;
    uint32_t bit;

    bit = 1UL << (bit_no & 31);
    reg = reg0 + (bit_no / 32) * sizeof(uint32_t);
    gic400_write_distributor(gic, reg, bit);

    return SUCCESS;
}

errno_t gic400_write_dist_bit(struct gic400 *gic, uint16_t bit_no,
                              uintptr_t reg0)
{
    errno_t ret;

    if (is_valid_interrupt_no(gic, bit_no)) {
        ret = write_dist_bit(gic, bit_no, reg0);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static void write_dist_byte(struct gic400 *gic, uint16_t byte_no,
                            uintptr_t reg0, uint8_t byte_data)
{
    uint32_t d;
    uint32_t mask;
    uint32_t shift_ct;
    uintptr_t reg;

    shift_ct = (byte_no & 3) * 8;
    mask = ~((uint32_t)0xff << shift_ct);
    reg = reg0 + (byte_no / 4) * sizeof(uint32_t);

    d = gic400_read_distributor(gic, reg);
    d = (d & mask) | ((uint32_t)byte_data << shift_ct);
    gic400_write_distributor(gic, reg, d);
}

uint32_t gic400_ack(struct gic400 *gic)
{
    uint32_t iar;

    if (gic != NULL) {
        iar = gic400_read_cpuif(gic, GICC_IAR);
    } else {
        iar = ~(uint32_t)0;
    }

    return iar;
}

errno_t gic400_eoi(struct gic400 *gic, uint32_t iar)
{
    gic400_write_cpuif(gic, GICC_EOIR, iar);

    return SUCCESS;
}

errno_t gic400_deactivate(struct gic400 *gic, uint32_t iar)
{
    gic400_write_cpuif(gic, GICC_DIR, iar);

    return SUCCESS;
}

errno_t gic400_eoi_and_deactivate(struct gic400 *gic, uint32_t iar)
{
    gic400_write_cpuif(gic, GICC_EOIR, iar);
    gic400_write_cpuif(gic, GICC_DIR, iar);

    return SUCCESS;
}

errno_t gic400_enable_interrupt(struct gic400 *gic, uint16_t intr_no)
{
    errno_t ret;

    ret = gic400_write_dist_bit(gic, intr_no, GICD_ISENABLER(0));

    return ret;
}

errno_t gic400_disable_interrupt(struct gic400 *gic, uint16_t intr_no)
{
    errno_t ret;

    ret = gic400_write_dist_bit(gic, intr_no, GICD_ICENABLER(0));
    sync_dist_io(gic);

    return ret;
}

static errno_t assert_sgi(struct gic400 *gic, uint8_t targets, uint16_t intr_no)
{
    uint32_t d;

    d = ((uint32_t)targets << 16) | (uint32_t)intr_no;
    gic400_write_distributor(gic, GICD_SGIR, d);

    return SUCCESS;
}

errno_t gic400_assert_sgi(struct gic400 *gic, uint8_t targets, uint16_t intr_no)
{
    errno_t ret;

    if ((gic != NULL) && (targets != 0) && (intr_no < 16)) {
        ret = assert_sgi(gic, targets, intr_no);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t assert_spi(struct gic400 *gic, uint16_t intr_no)
{
    errno_t ret;

    ret = write_dist_bit(gic, intr_no, GICD_ISPENDR(0));

    return ret;
}

static bool is_valid_spi_no(struct gic400 *gic, uint16_t intr_no)
{
    bool ret;

    if ((intr_no >= 32) && (intr_no < gic->nr_interrupts)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

errno_t gic400_assert_spi(struct gic400 *gic, uint16_t intr_no)
{
    errno_t ret;

    if ((gic != NULL) && is_valid_spi_no(gic, intr_no)) {
        ret = assert_spi(gic, intr_no);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t configure_icfg(struct gic400 *gic, uint16_t intr_no, uint32_t d)
{
    uint32_t d0;
    uint32_t mask;
    uint32_t shift_ct;
    uintptr_t reg;

    reg = GICD_ICFGR(intr_no / 16);
    shift_ct = (intr_no % 16) * 2;
    mask = ~((uint32_t)BITS(1, 0) << shift_ct);
    d <<= shift_ct;

    d0 = gic400_read_distributor(gic, reg);
    d |= d0 & mask;
    gic400_write_distributor(gic, reg, d);

    return SUCCESS;
}

static errno_t configure(struct gic400 *gic, uint16_t intr_no,
                         const struct gic400_interrupt_configuration *config)
{
    errno_t ret;
    uint32_t d;

    write_dist_bit(gic, intr_no, GICD_ICENABLER(0));
    sync_dist_io(gic);
    write_dist_byte(gic, intr_no, GICD_ITARGETSR(0), config->targets);
    d = config->priority << gic->priority.shift_ct;
    write_dist_byte(gic, intr_no, GICD_IPRIORITYR(0), d);

    if (config->flag.edge == 0) {
        d = 0;
    } else {
        d = 2;
    }
    ret = configure_icfg(gic, intr_no, d);

    return ret;
}

static errno_t
configure_interrupt(struct gic400 *gic, uint16_t intr_no,
                    const struct gic400_interrupt_configuration *config)
{
    errno_t ret;
    uint32_t lock;

    lock = cpu_lock_interrupts();
    gic400_lock(gic);

    ret = configure(gic, intr_no, config);

    gic400_unlock(gic);
    cpu_unlock_interrupts(lock);

    return ret;
}

static errno_t
validate_parameters(const struct gic400 *gic, uint16_t intr_no,
                    const struct gic400_interrupt_configuration *config)
{
    errno_t ret;

    if (! is_valid_interrupt_no(gic, intr_no)) {
        ret = -EINVAL;
    } else if (config == NULL) {
        ret = -EINVAL;
    } else if (config->targets == 0) {
        ret = -EINVAL;
    } else if (config->priority > gic->priority.max) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t
gic400_configure_interrupt(struct gic400 *gic, uint16_t intr_no,
                           const struct gic400_interrupt_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(gic, intr_no, config);
    if (ret == SUCCESS) {
        ret = configure_interrupt(gic, intr_no, config);
    }

    return ret;
}
