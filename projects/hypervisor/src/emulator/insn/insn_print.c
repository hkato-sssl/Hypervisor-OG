/*
 * emulator/insn/insn_print.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/emulator/insn.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void print_unknown(const struct insn *insn)
{
    printk("<Unknown instruction type>\n");
}

static void print_null(const struct insn *insn)
{
    printk("<INSN_TYPE_NULL>\n");
    printk("\n");
}

static void print_ldr(const struct insn *insn)
{
    printk("<INSN_TYPE_LDR>\n");
    printk("    va: 0x%016llx\n", insn->op.ldr.va);
    printk("    pa: 0x%016llx\n", insn->op.ldr.pa);
    if (insn->op.str.flag.isv == 0) {
        printk("offset: 0x%016llx(%lld)\n", insn->op.ldr.offset,
               insn->op.ldr.offset);
    }
    printk("  size: %u\n", insn->op.ldr.size);
    if (insn->op.str.flag.isv == 0) {
        if (insn->op.ldr.gpr.src == 31) {
            printk("   src: SP\n");
        } else {
            printk("   src: X%u\n", insn->op.ldr.gpr.src);
        }
    }

    if (insn->op.ldr.flag.wreg == 0) {
        if (insn->op.ldr.gpr.dst == 31) {
            printk("   dst: XZR\n");
        } else {
            printk("   dst: X%u\n", insn->op.ldr.gpr.dst);
        }
    } else {
        if (insn->op.ldr.gpr.dst == 31) {
            printk("   dst: WZR\n");
        } else {
            printk("   dst: W%u\n", insn->op.ldr.gpr.dst);
        }
    }
    printk("  flag: isv=%u, wreg=%u, sign=%u, post=%u, pre=%u\n",
           insn->op.ldr.flag.isv, insn->op.ldr.flag.wreg,
           insn->op.ldr.flag.sign, insn->op.ldr.flag.post,
           insn->op.ldr.flag.pre);
    printk("\n");
}

static void print_str(const struct insn *insn)
{
    printk("<INSN_TYPE_STR>\n");
    printk("    va: 0x%016llx\n", insn->op.str.va);
    printk("    pa: 0x%016llx\n", insn->op.str.pa);
    if (insn->op.str.flag.isv == 0) {
        printk("offset: 0x%016llx(%lld)\n", insn->op.str.offset,
               insn->op.str.offset);
    }
    printk("  size: %u\n", insn->op.str.size);
    if (insn->op.str.flag.wreg == 0) {
        if (insn->op.str.gpr.src == 31) {
            printk("   src: XZR(0x%016llx)\n", insn_str_src_value(insn));
        } else {
            printk("   src: X%u(0x%016llx)\n", insn->op.str.gpr.src,
                   insn_str_src_value(insn));
        }
    } else {
        if (insn->op.str.gpr.src == 31) {
            printk("   src: WZR(0x%016llx)\n", insn_str_src_value(insn));
        } else {
            printk("   src: W%u(0x%016llx)\n", insn->op.str.gpr.src,
                   insn_str_src_value(insn));
        }
    }
    if (insn->op.str.flag.isv == 0) {
        if (insn->op.str.gpr.dst == 31) {
            printk("   dst: SP\n");
        } else {
            printk("   dst: X%u\n", insn->op.str.gpr.dst);
        }
    }
    printk("  flag: isv=%u, wreg=%u, sign=%u, post=%u, pre=%u\n",
           insn->op.str.flag.isv, insn->op.str.flag.wreg,
           insn->op.str.flag.sign, insn->op.str.flag.post,
           insn->op.str.flag.pre);
    printk("\n");
}

void insn_print(const struct insn *insn)
{
    switch (insn->type) {
    case INSN_TYPE_NULL:
        print_null(insn);
        break;
    case INSN_TYPE_LDR:
        print_ldr(insn);
        break;
    case INSN_TYPE_STR:
        print_str(insn);
        break;
    default:
        print_unknown(insn);
        break;
    }
}
