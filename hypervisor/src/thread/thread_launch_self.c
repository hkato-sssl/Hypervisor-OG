/*
 * thread/thread_launch_self.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64.h"
#include "hypervisor/thread.h"
#include "thread_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static volatile void launch_self(uintptr_t *params, const struct thread_parameter *parameter)
{
    int i;

    params[THREAD_PARAMETER_ENTRY] = (uintptr_t)(parameter->entry);
    for (i = 0; i < NR_THREAD_ARGS; ++i) {
        params[THREAD_PARAMETER_ARG(i)] = parameter->args[i];
    }

    thread_start();
}

errno_t thread_launch_self(const struct thread_parameter *parameter)
{
    errno_t ret;
    uint32_t no;
    uintptr_t *params;

    no = aarch64_cpu_no();
    params = thread_parameter_array(no);
    if (params == NULL) {
        ret = -EPERM;
    } else if (parameter->entry == NULL) {
        ret = -EINVAL;
    } else {
        launch_self(params, parameter);
    }

    return ret;
}

