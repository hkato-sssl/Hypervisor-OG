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

#include <stdint.h>
#include "lib/slist.h"
#include "lib/system/errno.h"

/* defines */

#define HVC_SERVICE_ID(imm, c0, c1, c2, c3)         \
            (((uint64_t)(imm) << 32)            |   \
             ((uint64_t)(uint8_t)(c0) << 24)    |   \
             ((uint64_t)(uint8_t)(c1) << 16)    |   \
             ((uint64_t)(uint8_t)(c2) << 8)     |   \
             ((uint64_t)(uint8_t)(c3)))

/* types */

struct insn;
struct hvc_service;

typedef errno_t (*hvc_server_t)(const struct insn *insn, const struct hvc_service *service);

struct hvc_service {
    struct slist_node   node;
    uint64_t            id;
    hvc_server_t        server;
    void                *arg;
};

struct hvc_service_configuration {
    uint64_t            id;
    hvc_server_t        server;
    void                *arg;
};

/* variables */

/* functions */

errno_t hvc_service_initialize(struct hvc_service *service, const struct hvc_service_configuration *config);
errno_t hvc_service_append_slist(struct slist *list, struct hvc_service *service);
errno_t hvc_server(const struct insn *insn, const struct slist *service_list);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_HVC_H */

