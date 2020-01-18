/*
 * emulator/insn/insn_emulate_str.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/assert.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t emulate_str(const struct insn *insn)
{
    errno_t ret;

    ret = insn_emulate_indexed_addressing(insn);
    if (ret == SUCCESS) {
        ret = vpc_update_pc(insn->vpc);
    }

    return ret;
}

errno_t insn_emulate_str(const struct insn *insn)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_STR) {
        ret = emulate_str(insn);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

