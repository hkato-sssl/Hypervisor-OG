/*
 * test/vm/emulator_02.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t emulator_02(const struct insn *insn, void *arg)
{
    errno_t ret;

    printk("<%s>\n", __func__);
    insn_print(insn);

    ret = -ENOSYS;

    return ret;
}

