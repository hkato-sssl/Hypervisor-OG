/*
 * emulator/vgic400/vgic400_distributor_igroupr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_igroupr(struct vgic400 *gic, const struct insn *insn)
{
    errno_t ret;

    if (is_aligned_word_access(insn)) {
        /* RAZ/WI */
        if (insn->type == INSN_TYPE_LDR) {
            vpc_load_to_gpr_w(insn, 0);
        }
        ret = SUCCESS;
    } else {
        ret = -EPERM;
    }

    return ret;
}
