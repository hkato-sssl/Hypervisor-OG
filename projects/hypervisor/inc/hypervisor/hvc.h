// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/hvc.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_HVC_H
#define HYPERVISOR_HVC_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/slist.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

#define HVC_ERRNO(e) (uint64_t)(int64_t)(e)

/* types */

struct vpc;
struct insn;
struct hvc_service;

typedef errno_t (*hvc_server_t)(const struct insn *insn,
                                const struct hvc_service *service);

struct hvc_service {
    struct slist_node node;
    uint64_t id;
    hvc_server_t server;
    void *arg;
};

struct hvc_service_configuration {
    uint64_t id;
    hvc_server_t server;
    void *arg;
};

/* variables */

/* functions */

errno_t hvc_service_initialize(struct hvc_service *service,
                               const struct hvc_service_configuration *config);
errno_t hvc_service_append_slist(struct slist *list,
                                 struct hvc_service *service);
errno_t hvc_server(const struct insn *insn, const struct slist *service_list);
uint64_t hvc_service_id(uint16_t imm, const char *name);
errno_t hvc_set_result(struct vpc *vpc, uint64_t result);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_HVC_H */
