/*
 * hypervisor/emulate.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_EMULATE_H
#define HYPERVISOR_EMULATE_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

/* types */

struct vpc;

typedef errno_t (*emulate_func_t)(struct vpc *);

/* variables */

/* functions */

errno_t emulate_exception(struct vpc *vpc);

struct emulate_ops {
    emulate_func_t      irq;
    emulate_func_t      fiq;

    struct {
        emulate_func_t  svc;
        emulate_func_t  hvc;
        emulate_func_t  smc;
    } aarch64;

    struct {
        emulate_func_t  cpuif;
        emulate_func_t  distributor;
    } gic;
};

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATE_H */

