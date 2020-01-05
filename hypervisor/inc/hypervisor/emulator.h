/*
 * hypervisor/emulator.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_EMULATOR_H
#define HYPERVISOR_EMULATOR_H

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
struct insn;

typedef errno_t (*emulator_func_t)(struct vpc *);

struct emulator_ops {
    emulator_func_t     irq;
    emulator_func_t     fiq;

    struct {
        emulator_func_t svc;
        emulator_func_t hvc;
        emulator_func_t smc;
        emulator_func_t data_abort;
    } aarch64;
};

/* variables */

/* functions */

uint64_t emulator_ldrsb(uint64_t d, const struct insn *insn);
uint64_t emulator_ldrsw(uint64_t d, const struct insn *insn);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_H */

