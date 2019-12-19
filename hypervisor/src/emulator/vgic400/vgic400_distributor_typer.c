/*
 * emulator/vgic400/vgic400_distributor_typer.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_typer(struct vgic400 *vgic, const struct vpc_memory_access *access)
{
    uint64_t d;
    uint64_t nr_cpus;

    /* a write operation will be ignored */

    if (access->request.type == VPC_READ_ACCESS) {
        d = VGIC400_READ32(access->request.addr);
        nr_cpus = access->vpc->owner->nr_procs;
        d = (d & ~(uint32_t)BITS(7, 5)) | (nr_cpus << 5);
        vpc_load_to_gpr_w(access, d);
    }

    return SUCCESS;
}

