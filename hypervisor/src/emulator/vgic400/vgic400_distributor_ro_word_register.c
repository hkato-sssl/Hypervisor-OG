/*
 * emulator/vgic400/vgic400_distributor_ro_word_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_ro_word_register(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;
    uint64_t d;

    if (is_aligned_word_access(insn)) {
        if (insn->type == INSN_TYPE_LDR) {
            d = VGIC400_READ32(insn->op.ldr.ipa);
            vpc_emulate_ldr(insn, d);
        }
        /* write operation will be ignored */
        ret = SUCCESS;
    } else {
        ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
    }

    return ret;
}

