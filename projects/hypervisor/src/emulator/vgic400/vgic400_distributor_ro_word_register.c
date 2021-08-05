/*
 * emulator/vgic400/vgic400_distributor_ro_word_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "vgic400_local.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_ro_word_register(struct vgic400 *vgic,
                                             const struct insn *insn)
{
    errno_t ret;
    uint64_t d;

    if (is_aligned_word_access(insn)) {
        if (insn->type == INSN_TYPE_LDR) {
            d = VGIC400_READ32(insn->op.ldr.pa);
            ret = insn_emulate_ldr(insn, d);
        } else {
            /* write operation will be ignored */
            ret = insn_emulate_str(insn);
        }
    } else {
        ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
    }

    return ret;
}
