/*
 * emulator/vgic400emulator/vgic400_assert_virtual_spi.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/aarch64.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "driver/system/cpu.h"
#include "hypervisor/parameter.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_exposable(const struct vgic400 *vgic, uint16_t interrupt_no)
{
    bool result;
    uint32_t no;
    uint32_t bit;

    no = interrupt_no - vgic->virtual_spi.base_no;
    bit = 1UL << no;
    if (((vgic->virtual_spi.ienabler & bit) != 0) && (vgic->virtual_spi.ipriorityr[no] < vgic->priority_mask)) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

static errno_t assert_virtual_spi(struct vpc *vpc, struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;

    vgic400_lock(vgic);

    vgic->virtual_spi.ipendr |= 1UL << (interrupt_no - vgic->virtual_spi.base_no);

    if ((! vgic->virtual_spi.asserting) && is_exposable(vgic, interrupt_no)) {
        ret = vgic400_accept_virtual_spi(vpc, vgic);
    } else {
        ret = SUCCESS;  /* no work */
    }

    vgic400_unlock(vgic);

    return ret;
}

static errno_t validate_parameters(struct vpc *vpc, struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;
    uint32_t base;

    if (vgic->boolean.virtual_spi) {
        base = vgic->virtual_spi.base_no;
        if ((interrupt_no >= base) && (interrupt_no < (base + 32))) {
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EPERM;
    }

    return ret;
}

errno_t vgic400_assert_virtual_spi(struct vpc *vpc, struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;

    ret = validate_parameters(vpc, vgic, interrupt_no);
    if (ret == SUCCESS) {
        ret = assert_virtual_spi(vpc, vgic, interrupt_no);
    }

    return ret;
}

