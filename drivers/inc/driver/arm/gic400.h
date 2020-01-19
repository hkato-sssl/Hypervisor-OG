/*
 * driver/arm/gic400.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_ARM_GIC400_H
#define DRIVER_ARM_GIC400_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/spin_lock.h"

/* defines */

#define NR_GIC400_SGIS          16
#define NR_GIC400_PPIS          16
#define NR_GIC400_SPIS          480
#define NR_GIC400_INTERRUPTS    (NR_GIC400_SGIS + NR_GIC400_PPIS + NR_GIC400_SPIS)

/* types */

struct gic400_base_register {
    void    *distributor;
    void    *cpuif;
    void    *virtif_control;
};

struct gic400_configuration {
    struct gic400_base_register base;
    struct {
        bool    priority_drop;
        bool    virtualization;
    } boolean;
};

typedef intptr_t (*gic400_handler_t)(void *arg, uint32_t iar);

struct gic400 {
    spin_lock_t         lock;
    struct gic400_configuration config;
    struct {
        uint8_t         max;
        uint8_t         shift_ct;
    } priority;
    uint16_t            nr_interrupts;
    uint16_t            nr_list_registers;
};

struct gic400_interrupt_configuration {
    uint8_t             targets;
    uint8_t             priority;
    struct {
        uint8_t         group1:1;
        uint8_t         edge:1;
    } flag;
};

struct gic400_virtual_interrupt {
    uint8_t             priority;
    uint16_t            physical_id;
    uint16_t            virtual_id;
};

/* variables */

/* inline functions */

static inline void *gic400_distributor_register_base(const struct gic400 *gic)
{
    return gic->config.base.distributor;
}

/* functions */

errno_t gic400_init(struct gic400 *gic, const struct gic400_configuration *config);
uint32_t gic400_ack(struct gic400 *gic);
void gic400_eoi(struct gic400 *gic, uint32_t iar);
void gic400_deactivate(struct gic400 *gic, uint32_t iar);
errno_t gic400_enable_interrupt(struct gic400 *gic, uint16_t intr_no);
errno_t gic400_disable_interrupt(struct gic400 *gic, uint16_t intr_no);
errno_t gic400_assert_sgi(struct gic400 *gic, uint8_t targets, uint16_t intr_no);
errno_t gic400_assert_spi(struct gic400 *gic, uint16_t intr_no);
errno_t gic400_set_priority_mask(struct gic400 *gic, uint32_t priority);
errno_t gic400_enable_interrupt(struct gic400 *gic, uint16_t intr_no);
errno_t gic400_disable_interrupt(struct gic400 *gic, uint16_t intr_no);
errno_t gic400_configure_interrupt(struct gic400 *gic, uint16_t intr_no, const struct gic400_interrupt_configuration *config);

/* for virtualization */

errno_t gic400_inject_virtual_interrupt(struct gic400 *gic, const struct gic400_virtual_interrupt *vintr);

/* for debugging */

void gic400_dump_ns_cpuif(const struct gic400 *gic);
void gic400_dump_ns_distributor(const struct gic400 *gic);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_GIC400_H */

