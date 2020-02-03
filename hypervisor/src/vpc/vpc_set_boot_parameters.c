/*
 * vpc/vpc_set_boot_parameters.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/vpc.h"
#include "vpc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void set_aarch64(struct vpc *vpc)
{
    /* SPSR:
     *    D, A, I, F = 1
     *    M[4:0] = 0x05 - AArch64 EL1h
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_D | PSTATE_A | PSTATE_I | PSTATE_F | 0x05;
    vpc->regs[VPC_HCR_EL2] = HCR_RW | HCR_IMO | HCR_VM;
}

static void set_aarch32(struct vpc *vpc)
{
    /* SPSR:
     *    I, F = 1 - IRQ and FIQ are masked
     *    M[4:0] = 0x13 - AArch32 Supervisor mode
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_I | PSTATE_F | 0x13;
    vpc->regs[VPC_HCR_EL2] = HCR_IMO | HCR_VM;
}

void vpc_set_boot_parameters(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    vpc->regs[VPC_PC] = boot->pc;
    vpc->regs[VPC_X0] = boot->arg;

    if (boot->arch == VPC_ARCH_AARCH64) {
        set_aarch64(vpc);
    } else {
        set_aarch32(vpc);
    }
}

