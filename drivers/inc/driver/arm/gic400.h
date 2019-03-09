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
#include "lib/system/spin_lock.h"

/* defines */

#define NR_GIC400_SGIS          16
#define NR_GIC400_PPIS          16
#define NR_GIC400_SPIS          480
#define NR_GIC400_INTERRUPTS    (NR_GIC400_SGIS + NR_GIC400_PPIS + NR_GIC400_SPIS)

/* types */

struct gic400_base_register {
        uintptr_t   *dist;      /* distributor */
        uintptr_t   *cpuif;     /* CPU interface */
};

struct gic400_configuration {
        struct gic400_base_register base;
};

typedef void (*gic400_handler_t)(uint32_t iar, void *arg);

struct gic400 {
        spin_lock_t         lock;
        struct gic400_configuration config;
        struct {
            uint8_t         max;
            uint8_t         shift_ct;
        } priority;
        uint8_t             nr_interrupts;
        gic400_handler_t    handlers[NR_GIC400_INTERRUPTS];
};

struct gic400_interrupt_configuration {
    uint8_t             targets;
    uint8_t             priority;
    struct {
        uint8_t         group1:1;
        uint8_t         edge:1;
    } flag;

    gic400_handler_t    handler;
    void                *arg;
};

/* variables */

/* functions */

errno_t gic400_init(struct gic400 *gic, struct gic400_configuration const *config);
errno_t gic400_enable_interrupt(struct gic400 *gic, uint16_t intr_no);
errno_t gic400_disable_interrupt(struct gic400 *gic, uint16_t intr_no);
errno_t gic400_ack(struct gic400 *gic);
errno_t gic400_eoi(struct gic400 *gic);
errno_t gic400_register_handler(struct gic400 *gic, uint16_t intr_no, gic400_handler_t handler, void *arg);
errno_t gic400_configure_interrupt(struct gic400 *gic);
errno_t gic400_assert_sgi(struct gic400 *gic, uint8_t targets, uint16_t intr_no);
errno_t gic400_assert_interrupt(struct gic400 *gic, uint16_t intr_no);
uint8_t gic400_set_priority(struct gic400 *gic, uint8_t priority);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_GIC400_H */

