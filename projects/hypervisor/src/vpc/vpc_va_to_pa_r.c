/*
 * vpc/vpc_va_to_pa_r.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "driver/aarch64/at.h"
#include "driver/system/cpu.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "vpc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t va_to_pa_r(const struct vpc *vpc, uint64_t *pa, uint64_t va)
{
    errno_t ret;
    uint64_t par;

    if (is_el0(vpc)) {
        par = aarch64_at_s12e0r(va);
    } else {
        par = aarch64_at_s12e1r(va);
    }

    if ((par & BIT(0)) == 0) {
        *pa = (par & BITS(47, 12)) | (va & BITS(11, 0));
        ret = SUCCESS;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static bool is_translatable_vpc(const struct vpc *vpc)
{
    bool valid;
    uint8_t physical_no;

    physical_no = cpu_no();
    if (vpc->vm->proc_map.virtual[physical_no] == vpc->proc_no) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t vpc_va_to_pa_r(const struct vpc *vpc, uint64_t *pa, uint64_t va)
{
    errno_t ret;

    if (is_translatable_vpc(vpc)) {
        ret = va_to_pa_r(vpc, pa, va);
    } else {
        ret = -EPERM;
    }

    return ret;
}

