/*
 * emulator/vgic400/vgic400_distributor_igroupr.c
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

errno_t vgic400_distributor_igroupr(struct vgic400 *gic, const struct vpc_memory_access *access)
{
    errno_t ret;

    if (is_aligned_word_access(access)) {
        /* RAZ/WI */
        if (access->request.type == VPC_READ_ACCESS) {
            vpc_load_to_gpr_w(access, 0);
        }
        ret = SUCCESS;
    } else {
        ret = -EPERM;
    }

    return ret;
}
