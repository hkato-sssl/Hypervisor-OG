/*
 * soc/soc_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vm.h"
#include "hypervisor/soc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t initialize(struct soc *soc, const struct soc_configuration *soc_config)
{
    errno_t ret;
    struct vm_configuration config;

    memset(soc, 0, sizeof(*soc));

    spin_lock_init(&(soc->lock));
    soc->chip = soc_config->chip;
    soc->ops = soc_config->ops;

    memset(&config, 0, sizeof(config));
    config.soc = soc;
    config.nr_procs = soc_config->nr_procs;
    config.stage2 = &(soc->stage2);

    ret = vm_initialize(&(soc->vm), &config);

    return ret;
}


static errno_t validate_parameters(struct soc *soc, const struct soc_configuration *config)
{
    errno_t ret;

    if ((soc != NULL) && (config != NULL) && (config->chip != NULL) && (config->ops != NULL)) {
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t soc_initialize(struct soc *soc, const struct soc_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(soc, config);
    if (ret == SUCCESS) {
        ret = initialize(soc, config);
    }

    return ret;
}

