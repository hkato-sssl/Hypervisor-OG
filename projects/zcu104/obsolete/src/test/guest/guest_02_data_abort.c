// SPDX-License-Identifier: Apache-2.0
/*
 * test/guest/guest_02_data_abort.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

int test_no;

/* functions */

errno_t guest_02_data_abort(const struct insn *insn, void *arg)
{
    printk("#%d: pc=%p\n", ++test_no, insn->vpc->regs[VPC_PC]);
    insn_print(insn);
    vpc_update_pc(insn->vpc);

    return SUCCESS;
}

