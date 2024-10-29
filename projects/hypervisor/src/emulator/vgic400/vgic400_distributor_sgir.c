// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_distributor_sgir.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t write_sgir(struct vgic400 *vgic, const struct insn *insn, uint32_t value)
{
    errno_t ret;
    uint32_t v_target_list;
    uint32_t p_target_list;
    uint32_t filter;

    filter = BF_EXTRACT(value, 25, 24);
    switch (filter) {
    case 0:
        v_target_list = BF_EXTRACT(value, 23, 16);
        p_target_list = vgic400_v2p_cpu_map_b(v_target_list, insn->vpc->vm);
        if (p_target_list != 0) {
            value = (value & ~(uint32_t)BITS(23, 16)) | (p_target_list << 16);
            VGIC400_WRITE32(insn->op.str.pa, value);
        }
        ret = insn_emulate_str(insn);
        break;

    default:
        printk("#### Unsupported TragetListFilet(=%u).\n", filter);
        ret = -ENOTSUP;
    }

    return ret;
}

errno_t vgic400_distributor_sgir(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;
    uint32_t value;

    if (is_aligned_word_access(insn)) {
        if (insn->type == INSN_TYPE_STR) {
            value = (uint32_t)insn_str_src_value(insn);
            if (BF_EXTRACT(value, 3, 0) < 7) {
                ret = write_sgir(vgic, insn, value);
            } else {
                /* ignore invalid value writes. */
                ret = insn_emulate_str(insn);
            }
        } else {
            ret = insn_emulate_ldr(insn, 0);
        }
    } else {
        ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
    }

    return ret;
}
