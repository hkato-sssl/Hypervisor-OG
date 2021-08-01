/*
 * hypervisor/hvc/p128.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_HVC_P128_H
#define HYPERVISOR_HVC_P128_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/hvc.h"

/* defines */

#define MAX_P128_NAME_LEN   4

/* types */

struct hvc_p128_service {
    struct hvc_service      service;
    uint16_t                imm;                        /* hvc #imm */
    char                    name[MAX_P128_NAME_LEN];
    void                    *arg;
    uint32_t                nr_eps;
    struct p2p_packet_ep    **eps;
};

struct hvc_p128_service_configuration {
    uint16_t                imm;                        /* hvc #imm */
    char                    name[MAX_P128_NAME_LEN];
    void                    *arg;
    uint32_t                nr_eps;
    struct p2p_packet_ep    **eps;
};

/* variables */

/* functions */

errno_t hvc_p128_service_initialize(struct hvc_p128_service *service, const struct hvc_p128_service_configuration *config);
errno_t hvc_p128_server(const struct insn *insn, const struct hvc_service *service);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_HVC_P128_H */

