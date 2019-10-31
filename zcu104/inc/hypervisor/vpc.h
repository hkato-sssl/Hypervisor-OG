/*
 * hypervisor/vpc.h - the access library for Virtual Processor Core
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_VPC_H
#define HYPERVISOR_VPC_H

/* includes */

#include "vpc_register.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"

/* defines */

/* types */

struct vm;

enum vpc_arch { VPC_AARCH32, VPC_AARCH64 };

struct vpc_config {
    struct vm       *owner;
    uint64_t        *regs;

    uint8_t         proc_no;    // processor No.
    enum vpc_arch   arch;

    /* inital value of register */

    struct {
        uint64_t    pc;
        uint64_t    sp;
    } gpr;
};

struct vpc {
    struct vm   *owner;
    uint64_t    *regs;
    struct {
    bool        launched;
    } flag;
};

/* variables */

/* functions */

volatile void vpc_launch(struct vpc *vpc);
volatile void vpc_resume(struct vpc *vpc);

void vpc_load_ctx_fpu(uint64_t *regs);
void vpc_store_ctx_fpu(uint64_t *regs);
void vpc_load_ctx_system_register(uint64_t *regs);
void vpc_store_ctx_system_register(uint64_t *regs);

errno_t vpc_configure(struct vpc *vpc, const struct vpc_config *config);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VPC_H */

