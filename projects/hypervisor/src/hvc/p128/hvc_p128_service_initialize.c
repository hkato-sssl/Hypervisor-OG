// SPDX-License-Identifier: Apache-2.0
/*
 * hvc/p128/hvc_p128_service_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/hvc.h"
#include "hypervisor/hvc/p128.h"
#include "hypervisor/service/p2p_packet.h"
#include "lib/system/errno.h"
#include <stdint.h>
#include <string.h>

/* defines */

#define EOS        '\0'
#define MAX_NR_EPS 0xff

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t p128_service_initialize(
    struct hvc_p128_service *p128,
    const struct hvc_p128_service_configuration *p128_config)
{
    errno_t ret;
    struct hvc_service_configuration config;

    memset(p128, 0, sizeof(*p128));

    memset(&config, 0, sizeof(config));
    config.id = hvc_service_id(p128_config->imm, p128_config->name);
    config.server = hvc_p128_server;
    config.arg = p128;

    ret = hvc_service_initialize(&(p128->service), &config);
    if (ret == SUCCESS) {
        p128->imm = p128_config->imm;
        memcpy(p128->name, p128_config->name, sizeof(p128_config->name));
        p128->arg = p128_config->arg;
        p128->nr_eps = p128_config->nr_eps;
        p128->eps = p128_config->eps;
    }

    return ret;
}

static errno_t validate_eps(const struct hvc_p128_service_configuration *config)
{
    errno_t ret;
    uint32_t i;

    ret = SUCCESS;
    for (i = 0; i < config->nr_eps; ++i) {
        if (config->eps[i]->length != 128) {
            ret = -EPERM;
            break;
        }
    }

    return ret;
}

static errno_t
validate_parameters(const struct hvc_p128_service_configuration *config)
{
    errno_t ret;

    if (config->nr_eps == 0) {
        ret = -EINVAL;
    } else if (config->nr_eps > MAX_NR_EPS) {
        ret = -EINVAL;
    } else if (config->eps == NULL) {
        ret = -EINVAL;
    } else if (config->name[0] == EOS) {
        ret = -EINVAL;
    } else {
        ret = validate_eps(config);
    }

    return ret;
}

errno_t
hvc_p128_service_initialize(struct hvc_p128_service *service,
                            const struct hvc_p128_service_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(config);
    if (ret == SUCCESS) {
        ret = p128_service_initialize(service, config);
    }

    return ret;
}
