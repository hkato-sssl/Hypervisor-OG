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
struct emulate_ops;

enum vpc_arch { VPC_ARCH_AARCH32, VPC_ARCH_AARCH64 };

struct vpc {
    spin_lock_t     lock;
    struct vm       *owner;
    uint64_t        *regs;
    uint8_t         proc_no;    // processor No.

    struct {
        bool        launched;
    } boolean;

    struct {
        struct emulate_ops  *ops;
    } emulate;
};

struct vpc_configuration {
    struct vm               *owner;
    uint64_t                *regs;
    uint8_t                 proc_no;    // processor No.
    struct {
        struct emulate_ops  *ops;
    } emulate;
};

struct vpc_boot_configuration {
    enum vpc_arch   arch;
    uint64_t        pc;
    uint64_t        sp;
};

/* variables */

/* functions */

errno_t vpc_launch(struct vpc *vpc, struct vpc_boot_configuration const *boot);
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

