/*
 * emulator/vgic400/vgic400_distributor_typer.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t read_typer_w(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;
    uint64_t d;
    uint64_t nr_cpus;

    d = VGIC400_READ32(insn->op.ldr.ipa);
    nr_cpus = insn->vpc->vm->nr_procs;
    d = (d & ~(uint32_t)BITS(7, 5)) | ((nr_cpus - 1) << 5);

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

errno_t vgic400_distributor_typer(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;

    /* a write operation will be ignored */

    if (is_aligned_word_access(insn)) {
        if (insn->type == INSN_TYPE_LDR) {
            ret = read_typer_w(vgic, insn);
        } else {
            ret = insn_emulate_str(insn);
        }
    } else {
        ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
    }

    return ret;
}

