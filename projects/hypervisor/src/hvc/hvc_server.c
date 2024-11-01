// SPDX-License-Identifier: Apache-2.0
/*
 * hvc/hvc_server.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hvc_local.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/hvc.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vpc_register.h"
#include "lib/slist.h"
#include "lib/system/errno.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hvc_server(const struct insn *insn, const struct slist *service_list)
{
    errno_t ret;
    uint64_t id;
    struct hvc_service *service;

    id = ((uint64_t)(insn->op.system_call.imm) << 32)
         | (insn->vpc->regs[VPC_IP0] >> 32);
    service = slist_search_element(service_list, hvc_match_service, (void *)id);
    if (service != NULL) {
        ret = (service->server)(insn, service->arg);
    } else {
        vpc_dump(insn->vpc, 0);
        ret = hvc_set_result(insn->vpc, -ENOTSUP);
    }

    return ret;
}
