// SPDX-License-Identifier: Apache-2.0
/*
 * thread/thread_launch.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64.h"
#include "hypervisor/thread.h"
#include "lib/system/errno.h"
#include "thread_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t launch(uintptr_t *params,
                      const struct thread_parameter *parameter)
{
    int i;

    for (i = 0; i < NR_THREAD_ARGS; ++i) {
        params[THREAD_PARAMETER_ARG(i)] = parameter->args[i];
    }
    aarch64_dmb();
    params[THREAD_PARAMETER_ENTRY] = (uintptr_t)(parameter->entry);
    aarch64_dsb();
    aarch64_sev();

    return SUCCESS;
}

errno_t thread_launch(uint32_t processor_no,
                      const struct thread_parameter *parameter)
{
    errno_t ret;
    uintptr_t *params;

    params = thread_parameter_array(processor_no);
    if (params == NULL) {
        ret = -EPERM;
    } else if (parameter->entry == NULL) {
        ret = -EINVAL;
    } else if (params[THREAD_PARAMETER_ENTRY] != 0) {
        ret = -EBUSY;
    } else {
        ret = launch(params, parameter);
    }

    return ret;
}
