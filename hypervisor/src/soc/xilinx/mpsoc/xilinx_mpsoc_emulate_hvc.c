/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_emulate_hvc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/service/hvcs.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t xilinx_mpsoc_emulate_hvc(const struct insn *insn)
{
    errno_t ret;
    struct xilinx_mpsoc *mpsoc;

    mpsoc = insn->vpc->vm->soc->chip;
    ret = hvcs_server(insn, &(mpsoc->hvc_service_list));

    return ret;
}
