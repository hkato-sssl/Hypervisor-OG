/*
 * emulate/insn/insn_parse_hvc.c
 *
 * (C) 2020 Hidekazu Kato
 */

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

errno_t parse_hvc(struct insn *insn, struct vpc *vpc)
{
    return -ENOTSUP;
}

errno_t insn_parse_hvc(struct insn *insn, struct vpc *vpc)
{
    errno_t ret;

    if (vpc_is_aarch64(vpc)) {
        ret = parse_hvc(insn, vpc);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}

