/*
 * vpc/vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

#define DEFAULT_SCTLR_EL1   (SCTLR_nTWE | SCTLR_nTWI | SCTLR_CP15BEN | SCTLR_SA0 | SCTLR_SA) 

/* types */

/* prototypes */

errno_t vpc_switch_to_el1(uint64_t *regs);

/* variables */

/* functions */

static errno_t init_system_register(struct vpc *vpc)
{
    uint64_t d;

    vpc->regs[VPC_SCTLR_EL1] = SCTLR_EL1_RES1 | DEFAULT_SCTLR_EL1;

    d = aarch64_read_midr_el1();
    vpc->regs[VPC_VPIDR_EL2] = d;
    vpc->regs[VPC_VMPIDR_EL2] = BIT(31) | vpc->proc_no;

    return SUCCESS;
}

static errno_t setup_aarch64(struct vpc *vpc)
{
    errno_t ret;

    /* SPSR:
     *    D, A, I, F = 1
     *    M[4:0] = 0x05 - AArch64 EL1h
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_D | PSTATE_A | PSTATE_I | PSTATE_F | 0x05;
    vpc->regs[VPC_HCR_EL2] = HCR_RW | HCR_VM;

    ret = init_system_register(vpc);

    return ret;
}

static errno_t setup_aarch32(struct vpc *vpc)
{
    errno_t ret;

    /* SPSR:
     *    I, F = 1 - IRQ and FIQ are masked
     *    M[4:0] = 0x13 - AArch32 Supervisor mode
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_I | PSTATE_F | 0x13;
    vpc->regs[VPC_HCR_EL2] = HCR_VM;

    ret = init_system_register(vpc);

    return ret;
}

static errno_t launch(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vm *vm;

    vpc->regs[VPC_PC] = boot->pc;
    vpc->regs[VPC_SP_EL1] = boot->sp;

    vm = vpc->vm;
    vpc->regs[VPC_VTTBR_EL2] = aarch64_stage2_vttbr_el2(vm->stage2);
    vpc->regs[VPC_VTCR_EL2] = aarch64_stage2_vtcr_el2(vm->stage2);

    if (boot->arch == VPC_ARCH_AARCH64) {
	setup_aarch64(vpc);
    } else {
	setup_aarch32(vpc);
    }

    thread_write_tls(TLS_CURRENT_VPC_REGS, (uint64_t)vpc->regs);
    thread_write_tls(TLS_CURRENT_VPC, (uint64_t)vpc);
    thread_write_tls(TLS_CURRENT_VM, (uint64_t)(vpc->vm));

    vpc->boolean.launched = true;
    vpc_load_ctx_system_register(vpc->regs);
    vpc_load_ctx_fpu(vpc->regs);
    ret = vpc_switch_to_el1(vpc->regs);

    return ret;
}

static bool is_valid_arch(enum vpc_arch arch)
{
    bool valid;

    if ((arch == VPC_ARCH_AARCH64) || (arch == VPC_ARCH_AARCH32)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t vpc_launch(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    if ((vpc != NULL) && (boot != NULL) && is_valid_arch(boot->arch) && (! vpc->boolean.launched)) {
        ret = launch(vpc, boot);
    } else {
        ret = -EBUSY;
    }

    return ret;
}

errno_t vpc_resume(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc != NULL) && (vpc->boolean.launched)) {
        ret = vpc_switch_to_el1(vpc->regs);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

