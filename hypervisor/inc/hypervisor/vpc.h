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
struct insn;

enum vpc_arch { VPC_ARCH_AARCH32, VPC_ARCH_AARCH64 };
typedef errno_t (*vpc_emulator_t)(const struct insn *insn, void *arg);
typedef errno_t (*vpc_exception_emulator_t)(struct vpc *);

struct vpc_exception_ops {
    vpc_exception_emulator_t        irq;
    vpc_exception_emulator_t        fiq;

    struct {
        vpc_exception_emulator_t    svc;
        vpc_exception_emulator_t    hvc;
        vpc_exception_emulator_t    smc;
        vpc_exception_emulator_t    data_abort;
    } aarch64;
};

struct vpc {
    spin_lock_t     lock;
    struct vm       *vm;
    uint64_t        *regs;
    uint8_t         proc_no;    // processor No.

    struct {
        bool        launched;
    } boolean;

    struct {
        const struct vpc_exception_ops  *ops;
    } exception;
};

struct vpc_configuration {
    struct vm       *vm;
    uint64_t        *regs;
    uint8_t         proc_no;    // processor No.
    struct {
        const struct vpc_exception_ops  *ops;
    } exception;
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
errno_t vpc_update_pc(struct vpc *vpc);
errno_t vpc_va_to_pa_r(const struct vpc *vpc, uint64_t *pa, uint64_t va);
errno_t vpc_read_instruction(const struct vpc *vpc, uint32_t *code, uint64_t va);

bool vpc_is_aarch64(const struct vpc *vpc);
bool vpc_is_aarch32(const struct vpc *vpc);
bool vpc_is_aarch32a(const struct vpc *vpc);
bool vpc_is_aarch32t(const struct vpc *vpc);

/* for debugging */

void vpc_dump(const struct vpc *vpc, unsigned int level);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VPC_H */

