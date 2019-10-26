/*
 * exception/handler_el2.c - the default exception handler
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/printk.h"
#include "driver/aarch64/exception.h"

/* defines */

/* types */

/* prototypes */

void exception_handler_el2(uint64_t *ei) __attribute__((weak));

/* variables */

/* functions */

static void dump_register(uint64_t *ei)
{
    printk("VECTOR: 0x%04x\n", ei[EXC_VECTOR]);
    printk("   ESR: 0x%08x\n", ei[EXC_ESR]);
    printk("   ELR: 0x%08x\n", ei[EXC_ELR]);
    printk("  SPSR: 0x%08x\n", ei[EXC_SPSR]);
    printk(" SP: 0x%08x  LR: 0x%08x\n", ei[EXC_SP], ei[EXC_LR]);
    printk(" X0: 0x%08x  X1: 0x%08x\n", ei[EXC_X0], ei[EXC_X1]);
    printk(" X2: 0x%08x  X3: 0x%08x\n", ei[EXC_X2], ei[EXC_X3]);
    printk(" X4: 0x%08x  X5: 0x%08x\n", ei[EXC_X4], ei[EXC_X5]);
    printk(" X6: 0x%08x  X7: 0x%08x\n", ei[EXC_X6], ei[EXC_X7]);
    printk(" X8: 0x%08x  X9: 0x%08x\n", ei[EXC_X8], ei[EXC_X9]);
    printk("X10: 0x%08x X11: 0x%08x\n", ei[EXC_X10], ei[EXC_X11]);
    printk("X12: 0x%08x X13: 0x%08x\n", ei[EXC_X12], ei[EXC_X13]);
    printk("X14: 0x%08x X15: 0x%08x\n", ei[EXC_X14], ei[EXC_X15]);
    printk("X16: 0x%08x X17: 0x%08x\n", ei[EXC_X16], ei[EXC_X17]);
    printk("X18: 0x%08x X19: 0x%08x\n", ei[EXC_X18], ei[EXC_X19]);
    printk("X20: 0x%08x X21: 0x%08x\n", ei[EXC_X20], ei[EXC_X21]);
    printk("X22: 0x%08x X23: 0x%08x\n", ei[EXC_X22], ei[EXC_X23]);
    printk("X24: 0x%08x X25: 0x%08x\n", ei[EXC_X24], ei[EXC_X25]);
    printk("X26: 0x%08x X27: 0x%08x\n", ei[EXC_X26], ei[EXC_X27]);
    printk("X28: 0x%08x X29: 0x%08x\n", ei[EXC_X28], ei[EXC_X29]);
    printk("\n");
}

void exception_handler_el2(uint64_t *ei)
{
    uint32_t ec;
    uint32_t il;
    uint32_t iss;
    const char *ec_msg;

    ec = (uint32_t)BF_EXTRACT(ei[EXC_ESR], 31, 26);
    il = (uint32_t)BF_EXTRACT(ei[EXC_ESR], 25, 25);
    iss = ei[EXC_ESR] & BITS(24, 0);
    ec_msg = aarch64_esr_ec_message((uint32_t)ec);

    printk("<%s>\n", __func__);
    printk("ESR: EC=0x%08x, IL=%d, ISS=0x%04x\n", ec, il, iss);
    printk("     %s\n", ec_msg);
    dump_register(ei);
}

