/*
 * emulator/insn/insn_print.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "hypervisor/emulator/insn.h"

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
    printk("   ipa: 0x%016llx\n", insn->op.ldr.ipa);
    printk("offset: 0x%016llx(%lld)\n", insn->op.ldr.offset, insn->op.ldr.offset);
    printk("  size: %u\n", insn->op.ldr.size);
    printk("   dst: %c%u\n", (insn->op.ldr.flag.wreg ? 'W' : 'X'), insn->op.ldr.gpr.dst);
    printk("  flag: wreg=%u, sign=%u, post=%u, wb=%u\n", insn->op.ldr.flag.wreg, insn->op.ldr.flag.sign, insn->op.ldr.flag.post, insn->op.ldr.flag.wb);
    printk("\n");
}

static void print_str(const struct insn *insn)
{
    uint8_t gpr;

    printk("<INSN_TYPE_STR>\n");
    gpr = insn->op.str.gpr.src;
    if (gpr < 31) {
        printk(" src: %c%u(0x%016x)\n", (insn->op.str.flag.wreg ? 'W' : 'X'), gpr, insn_str_src_value(insn));
    } else {
        printk(" src: %cZR(0x%016x)\n", (insn->op.str.flag.wreg ? 'W' : 'X'), insn_str_src_value(insn));
    }
    printk("    va: 0x%016llx\n", insn->op.str.va);
    printk("   ipa: 0x%016llx\n", insn->op.str.ipa);
    printk("offset: 0x%016llx(%lld)\n", insn->op.str.offset, insn->op.str.offset);
    printk("  size: %u\n", insn->op.str.size);
    printk("  flag: wreg=%u, sign=%u, post=%u, wb=%u\n", insn->op.str.flag.wreg, insn->op.str.flag.sign, insn->op.str.flag.post, insn->op.str.flag.wb);
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
