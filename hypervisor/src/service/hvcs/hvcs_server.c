/*
 * service/hvcs/hvcs_server.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/slist.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vpc_register.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/service/hvcs.h"
#include "hvcs_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hvcs_server(const struct insn *insn, const struct slist *service_list)
{
    errno_t ret;
    uint64_t id;
    struct hvcs_service *service;

    id = ((uint64_t)(insn->op.system_call.imm) << 32) | (insn->vpc->regs[VPC_IP0]>> 32);
    service = slist_search_element(service_list, hvcs_match_service, (void *)id);
    if (service != NULL) {
        ret = (service->server)(insn, service->arg);
    } else {
        ret= -ENOTSUP;
    }

    return ret;
}
