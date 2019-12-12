/*
 * vpc/vpc_emulate_aarch64_data_abort.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register/esr_el2_iss.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void generate_access_request(struct vpc *vpc, struct vpc_memory_access_request *req)
{
    uint64_t esr;

    esr = vpc->regs[VPC_ESR_EL2];
    req->access = ((esr & ISS_DATA_ABORT_WnR) != 0) ? VPC_WRITE_ACCESS : VPC_READ_ACCESS;
    req->size = 1 << ISS_DATA_ABORT_SAS(esr);
    req->addr = (vpc->regs[VPC_HPFAR_EL2] & BITS(39, 4)) << 8;
}

static errno_t emulate_aarch64_data_abort(struct vpc *vpc)
{
    errno_t ret;
    struct vpc_memory_access_request req;

    generate_access_request(vpc, &req);
    ret = vm_emulate_aarch64_memory_access(vpc, &req);

    return ret;
}

errno_t vpc_emulate_aarch64_data_abort(struct vpc *vpc)
{
    errno_t ret;
    uint64_t esr;

    esr = vpc->regs[VPC_ESR_EL2];
    if ((esr & ISS_DATA_ABORT_ISV) != 0) {
        ret = emulate_aarch64_data_abort(vpc);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}

