/*
 * emulator/vgic400/vgic400_distributor_ctlr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_ctlr(struct vgic400 *vgic, const struct vpc_memory_access *access)
{
    errno_t ret;

    /* a write operation will be ignored */

    if (is_aligned_word_access(access)) {
        if (access->request.type == VPC_READ_ACCESS) {
            vpc_load_to_gpr_w(access, 1);
        }
        ret = SUCCESS;
    } else {
        ret = vgic400_distributor_error(access, ERR_MSG_UNAUTH);
    }

    return ret;
}

