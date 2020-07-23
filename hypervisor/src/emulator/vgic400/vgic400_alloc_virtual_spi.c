/*
 * emulator/vgic400/vgic400_allocate_virtual_spi.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t allocate_virtual_spi(struct vgic400 *vgic, uint16_t *interrupt_no)
{
    errno_t ret;
    uint64_t n;

    vgic400_lock(vgic);

    n = 64 - aarch64_clz(vgic->virtual_spi.used);
    if (n < 32) {
        vgic->virtual_spi.used |= BIT(n);
        *interrupt_no = (uint16_t)(n + vgic->virtual_spi.base_no);
        ret = SUCCESS;
    } else {
        ret = -EBUSY;
    }

    vgic400_unlock(vgic);

    return ret;
}

errno_t vgic400_allocate_virtual_spi(struct vgic400 *vgic, uint16_t *interrupt_no)
{
    errno_t ret;

    if (vgic->boolean.virtual_spi) {
        ret = allocate_virtual_spi(vgic, interrupt_no);
    } else {
        ret = -EPERM;
    }

    return ret;
}

