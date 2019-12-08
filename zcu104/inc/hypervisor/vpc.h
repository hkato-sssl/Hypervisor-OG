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
#include "lib/system/spin_lock.h"

/* defines */

/* types */

struct vm;

enum vpc_arch { VPC_AARCH32, VPC_AARCH64 };

struct vpc {
    spin_lock_t     lock;
    struct vm       *owner;
    uint64_t        *regs;
    uint8_t         proc_no;    // processor No.
    struct {
        bool        launched;
    } boolean;
};

struct vpc_configuration {
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

/* variables */

/* functions */

errno_t vpc_launch(struct vpc *vpc);
errno_t vpc_resume(struct vpc *vpc);

void vpc_load_ctx_fpu(uint64_t *regs);
void vpc_store_ctx_fpu(uint64_t *regs);
void vpc_load_ctx_system_register(uint64_t *regs);
void vpc_store_ctx_system_register(uint64_t *regs);

errno_t vpc_configure(struct vpc *vpc, const struct vpc_configuration *config);

/* for debugging */

void vpc_dump(struct vpc const *vpc, unsigned int level);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VPC_H */

