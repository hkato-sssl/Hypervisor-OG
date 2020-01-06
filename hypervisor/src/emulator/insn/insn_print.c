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
    printk("  va: %p\n", insn->op.ldr.va);
    printk(" ipa: %p\n", insn->op.ldr.ipa);
    printk("size: %u\n", insn->op.ldr.size);
    printk(" dst: %c%u\n", (insn->op.ldr.flag.a32 ? 'W' : 'X'), insn->op.ldr.gpr.dst);
    printk("flag: sign=%u, a32=%u\n", insn->op.ldr.flag.sign, insn->op.ldr.flag.a32);
    printk("\n");
}

static void print_str(const struct insn *insn)
{
    printk("<INSN_TYPE_STR>\n");
    printk(" src: %c%u(0x%016x)\n", (insn->op.str.flag.a32 ? 'W' : 'X'), insn->op.str.gpr.src, insn_str_src_value(insn));
    printk("  va: %p\n", insn->op.str.va);
    printk(" ipa: %p\n", insn->op.str.ipa);
    printk("size: %u\n", insn->op.str.size);
    printk("flag: sign=%u, a32=%u\n", insn->op.str.flag.sign, insn->op.str.flag.a32);
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
