/*
 * test/guest/guest_02_data_abort.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t guest_02_data_abort(const struct insn *insn, void *arg)
{
    insn_print(insn);
    vpc_update_pc(insn->vpc);

    return SUCCESS;
}

