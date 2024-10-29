// SPDX-License-Identifier: Apache-2.0
/*
 * exception/aarch64_exception_dump.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/exception.h"
#include "lib/bit.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void aarch64_exception_dump(const uint64_t *ei)
{
    uint32_t ec;
    uint32_t il;
    uint32_t iss;
    const char *ec_msg;

    ec = (uint32_t)BF_EXTRACT(ei[EXC_ESR], 31, 26);
    il = (uint32_t)BF_EXTRACT(ei[EXC_ESR], 25, 25);
    iss = ei[EXC_ESR] & BITS(24, 0);
    ec_msg = aarch64_exception_esr_ec_message((uint32_t)ec);

    printk("VECTOR: 0x%04x\n", ei[EXC_VECTOR]);
    printk("   ESR: 0x%08x, EC=0x%02x, IL=%d, ISS=0x%07x\n", ei[EXC_ESR], ec,
           il, iss);
    printk("        %s.\n", ec_msg);
    printk("  SPSR: 0x%08x\n", ei[EXC_SPSR]);
    printk("   ELR: 0x%016lx\n", ei[EXC_ELR]);
    printk(" SP: 0x%016lx  LR: 0x%016lx\n", ei[EXC_SP], ei[EXC_LR]);
    printk(" X0: 0x%016lx  X1: 0x%016lx\n", ei[EXC_X0], ei[EXC_X1]);
    printk(" X2: 0x%016lx  X3: 0x%016lx\n", ei[EXC_X2], ei[EXC_X3]);
    printk(" X4: 0x%016lx  X5: 0x%016lx\n", ei[EXC_X4], ei[EXC_X5]);
    printk(" X6: 0x%016lx  X7: 0x%016lx\n", ei[EXC_X6], ei[EXC_X7]);
    printk(" X8: 0x%016lx  X9: 0x%016lx\n", ei[EXC_X8], ei[EXC_X9]);
    printk("X10: 0x%016lx X11: 0x%016lx\n", ei[EXC_X10], ei[EXC_X11]);
    printk("X12: 0x%016lx X13: 0x%016lx\n", ei[EXC_X12], ei[EXC_X13]);
    printk("X14: 0x%016lx X15: 0x%016lx\n", ei[EXC_X14], ei[EXC_X15]);
    printk("X16: 0x%016lx X17: 0x%016lx\n", ei[EXC_X16], ei[EXC_X17]);
    printk("X18: 0x%016lx X19: 0x%016lx\n", ei[EXC_X18], ei[EXC_X19]);
    printk("X20: 0x%016lx X21: 0x%016lx\n", ei[EXC_X20], ei[EXC_X21]);
    printk("X22: 0x%016lx X23: 0x%016lx\n", ei[EXC_X22], ei[EXC_X23]);
    printk("X24: 0x%016lx X25: 0x%016lx\n", ei[EXC_X24], ei[EXC_X25]);
    printk("X26: 0x%016lx X27: 0x%016lx\n", ei[EXC_X26], ei[EXC_X27]);
    printk("X28: 0x%016lx X29: 0x%016lx\n", ei[EXC_X28], ei[EXC_X29]);
    printk("\n");
}
