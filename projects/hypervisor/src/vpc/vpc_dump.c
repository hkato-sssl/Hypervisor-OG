/*
 * vpc/vpc_dump.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void vpc_dump(const struct vpc *vpc, unsigned int level)
{
    uint64_t *regs;

    printk("<VPC#%u>\n", vpc->proc_no);

    regs = vpc->regs;
    printk("  ESR_EL2: 0x%08x\n", regs[VPC_ESR_EL2]);
    printk("  FAR_EL2: 0x%016x\n", regs[VPC_FAR_EL2]);
    printk("HPFAR_EL2: 0x%016x\n", regs[VPC_HPFAR_EL2]);
    printk(" SPSR_EL2: 0x%08x\n", regs[VPC_SPSR_EL2]);
    printk("   SP_EL0: 0x%016lx\n", regs[VPC_SP_EL0]);
    printk("   SP_EL1: 0x%016lx\n", regs[VPC_SP_EL1]);
    printk(" PC: 0x%016lx  LR: 0x%016lx\n", regs[VPC_PC], regs[VPC_LR]);
    printk(" X0: 0x%016lx  X1: 0x%016lx\n", regs[VPC_X0], regs[VPC_X1]);
    printk(" X2: 0x%016lx  X3: 0x%016lx\n", regs[VPC_X2], regs[VPC_X3]);
    printk(" X4: 0x%016lx  X5: 0x%016lx\n", regs[VPC_X4], regs[VPC_X5]);
    printk(" X6: 0x%016lx  X7: 0x%016lx\n", regs[VPC_X6], regs[VPC_X7]);
    printk(" X8: 0x%016lx  X9: 0x%016lx\n", regs[VPC_X8], regs[VPC_X9]);
    printk("X10: 0x%016lx X11: 0x%016lx\n", regs[VPC_X10], regs[VPC_X11]);
    printk("X12: 0x%016lx X13: 0x%016lx\n", regs[VPC_X12], regs[VPC_X13]);
    printk("X14: 0x%016lx X15: 0x%016lx\n", regs[VPC_X14], regs[VPC_X15]);
    printk("X16: 0x%016lx X17: 0x%016lx\n", regs[VPC_X16], regs[VPC_X17]);
    printk("X18: 0x%016lx X19: 0x%016lx\n", regs[VPC_X18], regs[VPC_X19]);
    printk("X20: 0x%016lx X21: 0x%016lx\n", regs[VPC_X20], regs[VPC_X21]);
    printk("X22: 0x%016lx X23: 0x%016lx\n", regs[VPC_X22], regs[VPC_X23]);
    printk("X24: 0x%016lx X25: 0x%016lx\n", regs[VPC_X24], regs[VPC_X25]);
    printk("X26: 0x%016lx X27: 0x%016lx\n", regs[VPC_X26], regs[VPC_X27]);
    printk("X28: 0x%016lx X29: 0x%016lx\n", regs[VPC_X28], regs[VPC_X29]);
    printk("\n");
}
