/*
 * emulator/vgic400/vgic400_distributor_igroupr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_igroupr(struct vgic400 *vgic,
                                    const struct insn *insn)
{
    errno_t ret;

    if (is_aligned_word_access(insn)) {
        /* RAZ/WI */
        if (insn->type == INSN_TYPE_LDR) {
            ret = insn_emulate_ldr(insn, 0);
        } else {
            ret = insn_emulate_str(insn);
        }
    } else {
        ret = -EPERM;
    }

    return ret;
}
