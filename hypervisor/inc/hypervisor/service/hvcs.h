/*
 * hypervisor/service/hvcs.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_SERVICE_HVCS_H
#define HYPERVISOR_SERVICE_HVCS_H

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

#define HVCS_SERVICE_ID(imm, c0, c1, c2, c3)        \
            (((uint64_t)(imm) << 32)            |   \
             ((uint64_t)(uint8_t)(c0) << 24)    |   \
             ((uint64_t)(uint8_t)(c1) << 16)    |   \
             ((uint64_t)(uint8_t)(c2) << 8)     |   \
             ((uint64_t)(uint8_t)(c3)))

/* types */

struct insn;
struct p2p_packet_ep;
struct hvcs_service;

typedef errno_t (*hvcs_server_t)(const struct insn *insn, void *arg);

struct hvcs_service {
    struct slist_node   node;
    uint64_t            id;
    hvcs_server_t       server;
    void                *arg;
};

struct hvcs_service_configuration {
    uint64_t            id;
    hvcs_server_t       server;
    void                *arg;
};

/* variables */

/* functions */

errno_t hvcs_initialize_service(struct hvcs_service *service, const struct hvcs_service_configuration *config);
errno_t hvcs_register_service(struct slist *list, struct hvcs_service *service);
errno_t hvcs_server(const struct insn *insn, const struct slist *service_list);
errno_t hvcs_p2p_server(const struct insn *insn, struct p2p_packet_ep *ep);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SERVICE_HVCS_H */

