/*
 * soc/soc_initialize_vpc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t soc_initialize_vpc(struct soc *soc, struct vpc *vpc,
                           const struct vpc_configuration *config)
{
    errno_t ret;

    assert((soc != NULL) && (vpc != NULL) && (config != NULL));

    ret = vpc_initialize(vpc, config);
    if (ret == SUCCESS) {
        ret = vm_register_vpc(&(soc->vm), vpc);
    }

    return ret;
}
