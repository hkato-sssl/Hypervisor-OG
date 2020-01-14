/*
 * vpc/vpc_emulate_aarch64_data_abort.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t call_emulator(struct vpc *vpc, struct insn *insn)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = vm_search_region_trap(vpc->vm, insn->op.ldr.ipa);
    if (trap != NULL) {
        ret = (*(trap->emulator.handler))(insn, trap->emulator.arg);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}

errno_t vpc_emulate_aarch64_data_abort(struct vpc *vpc)
{
    errno_t ret;
    struct insn insn;

    ret = insn_parse_ldr_str(&insn, vpc);
    if (ret == SUCCESS) {
        ret = call_emulator(vpc, &insn);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

