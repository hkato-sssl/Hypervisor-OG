/*
 * driver/system/cpu.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_SYSTEM_CPU_H
#define DRIVER_SYSTEM_CPU_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>

/* defines */

/* types */

/* variables */

/* functions */

const uint8_t nr_cpus(void);
const uint8_t cpu_no(void);

bool lock_interrupts(void);
void unlock_interrupts(bool lock);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_SYSTEM_CPU_H */

