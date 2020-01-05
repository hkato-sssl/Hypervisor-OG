/*
 * vpc/vpc_post_operation.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/emulator/insn.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void emulate_ldr_post_operation(const struct insn *insn)
{
    /* no work */
}

static void emulate_str_post_operation(const struct insn *insn)
{
    /* no work */
}

void vpc_emulate_post_operation(const struct insn *insn)
{
    if (insn->type == INSN_TYPE_LDR) {
        emulate_ldr_post_operation(insn);
    } else if (insn->type == INSN_TYPE_STR) {
        emulate_str_post_operation(insn);
    }
}
