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
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_ro_word_register(struct vgic400 *vgic, const struct vpc_memory_access *access)
{
    errno_t ret;
    uint64_t d;

    if (is_aligned_word_access(access)) {
        if (access->request.type == VPC_READ_ACCESS) {
            d = VGIC400_READ32(access->request.addr);
            vpc_load_to_gpr_w(access, d);
        }
        /* write operation will be ignored */
        ret = SUCCESS;
    } else {
        ret = vgic400_distributor_error(access, ERR_MSG_UNAUTH);
    }

    return ret;
}

