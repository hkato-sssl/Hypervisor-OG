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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"

/* defines */

/* types */

struct vm;
struct vpc;
struct vpc_memory_access;

enum vpc_arch { VPC_ARCH_AARCH32, VPC_ARCH_AARCH64 };
typedef errno_t (*vpc_emulator_t)(struct vpc *vpc);
typedef errno_t (*vpc_memory_access_emulator_t)(const struct vpc_memory_access *access, void *arg);

struct vpc_emulator_ops {
    vpc_emulator_t      irq;
    vpc_emulator_t      fiq;

    struct {
        vpc_emulator_t  svc;
        vpc_emulator_t  hvc;
        vpc_emulator_t  smc;
        vpc_emulator_t  data_abort;
    } aarch64;
};

enum vpc_access_type {
    VPC_READ_ACCESS,
    VPC_WRITE_ACCESS
};

struct vpc_memory_access {
    struct vpc                  *vpc;   /* an originator */

    struct {
        enum vpc_access_type    type;
        uintptr_t               addr;
        uint8_t                 size;
        uint8_t                 gpr;    /* general purpose register */
        struct {
            uint8_t             sign:1; /* sign extension */
            uint8_t             a32:1;  /* access with a 32-bit wide register */
        } flag;
    } request;
};

struct vpc {
    spin_lock_t     lock;
    struct vm       *owner;
    uint64_t        *regs;
    uint8_t         proc_no;    // processor No.

    struct {
        bool        launched;
    } boolean;

    struct {
        const struct vpc_emulator_ops   *ops;
    } emulator;
};

struct vpc_configuration {
    struct vm       *owner;
    uint64_t        *regs;
    uint8_t         proc_no;    // processor No.
    struct {
        const struct vpc_emulator_ops   *ops;
    } emulator;
};

struct vpc_boot_configuration {
    enum vpc_arch   arch;
    uint64_t        pc;
    uint64_t        sp;
};

/* variables */

/* functions */

errno_t vpc_launch(struct vpc *vpc, const struct vpc_boot_configuration *boot);
errno_t vpc_resume(struct vpc *vpc);

void vpc_load_ctx_fpu(uint64_t *regs);
void vpc_store_ctx_fpu(uint64_t *regs);
void vpc_load_ctx_system_register(uint64_t *regs);
void vpc_store_ctx_system_register(uint64_t *regs);

errno_t vpc_configure(struct vpc *vpc, const struct vpc_configuration *config);
errno_t vpc_emulate_exception(struct vpc *vpc);
errno_t vpc_emulate_aarch64_data_abort(struct vpc *vpc);

void vpc_load_to_gpr_b(const struct vpc_memory_access *access, uint64_t d);
void vpc_load_to_gpr_h(const struct vpc_memory_access *access, uint64_t d);
void vpc_load_to_gpr_w(const struct vpc_memory_access *access, uint64_t d);

/* for debugging */

void vpc_dump(const struct vpc *vpc, unsigned int level);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VPC_H */

