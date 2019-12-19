/*
 * emulator/vgic400/vgic400_distributor_sgir.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t write_sgir(struct vgic400 *vgic, const struct vpc_memory_access *access)
{
    uint64_t d;
    uint64_t irq;
    uint64_t v_target_list;
    uint64_t p_target_list;

    d = gpr_value(access);
    irq = d & BITS(3, 0);
    if (irq < 8) {
        v_target_list = BF_EXTRACT(d, 32, 16);
        p_target_list = vgic400_v2p_cpu_map_b(v_target_list, access->vpc->owner);
        d = (d & ~(uint32_t)BITS(23, 16)) | (p_target_list << 16);
        VGIC400_WRITE32(access->request.addr, d);
    }

    return SUCCESS;
}

errno_t vgic400_distributor_sgir(struct vgic400 *vgic, const struct vpc_memory_access *access)
{
    errno_t ret;

    if (is_aligned_word_access(access)) {
        if (access->request.type == VPC_WRITE_ACCESS) {
            ret = write_sgir(vgic, access);
        } else {
            vpc_load_to_gpr_w(access, 0);
            ret = SUCCESS;
        }
    } else {
        vgic400_distributor_error(access, ERR_MSG_UNAUTH);
        ret = -EPERM;
    }

    return ret;
}

