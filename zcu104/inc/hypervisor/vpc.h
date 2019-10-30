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

/* defines */

/* types */

struct vm;

struct vpc_config {
    struct vm   *owner;
    uint64_t    *regs;

    uint8_t     proc_no;

    /* inital value of register */

    struct {
        struct {
            uint64_t    pc;
            uint64_t    sp;
        } gpr;
    
        struct {
            uint32_t    spsr;
        } spr;
    
        struct {
            uint32_t    midr;
            uint32_t    mpidr;
            uint32_t    sctlr;
        } sys;
    } register;
};

struct vpc {
    struct vm   *owner;
    uint64_t    *regs;
};

/* variables */

/* functions */

void vpc_launch(struct vpc *vpc);
void vpc_resume(struct vpc *vpc);
void vpc_load_ctx_fpu(uint64_t *regs);
void vpc_store_ctx_fpu(uint64_t *regs);
void vpc_load_ctx_system_register(uint64_t *regs);
void vpc_store_ctx_system_register(uint64_t *regs);

void vpc_default_configuration(struct vpc_config *config);
void vpc_configure(struct vpc *vpc, const struct vpc_config *config);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VPC_H */

