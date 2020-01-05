/*
 * test/vm/emulator_02.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"
#include "hypervisor/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void print_insn(const struct insn *insn)
{
    printk(" Access : %s\n", ((insn->type == INSN_TYPE_STR) ? "STR" : "LDR"));
    printk("Address : %018p\n", insn->op.ldr.ipa);
    printk("   Size : %u\n", insn->op.ldr.size);
    printk("   Sign : %u\n", insn->op.ldr.flag.sign);
}

errno_t emulator_02(struct vpc *vpc, const struct insn *insn)
{
    errno_t ret;

    printk("<%s>\n", __func__);
    print_insn(insn);

    ret = -ENOSYS;

    return ret;
}

