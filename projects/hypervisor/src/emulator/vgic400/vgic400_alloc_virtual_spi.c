/*
 * emulator/vgic400/vgic400_allocate_virtual_spi.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/emulator/vgic400.h"
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "vgic400_local.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t allocate_virtual_spi(struct vgic400 *vgic,
                                    uint16_t *interrupt_no, const char *name)
{
    errno_t ret;
    uint64_t n;

    vgic400_lock(vgic);

    n = 64 - aarch64_clz(vgic->virtual_spi.used);
    if (n < 32) {
        vgic->virtual_spi.used |= BIT(n);
        strncpy(vgic->virtual_spi.name[n], name, VGIC400_NAME_LEN);
        *interrupt_no = (uint16_t)(n + vgic->virtual_spi.base_no);
        if (name != NULL) {
            printk("VSPI#%u is allocated by '%s'.\n", *interrupt_no, name);
        } else {
            printk("VSPI#%u is allocated.\n", *interrupt_no);
        }
        ret = SUCCESS;
    } else {
        ret = -EBUSY;
    }

    vgic400_unlock(vgic);

    return ret;
}

errno_t vgic400_allocate_virtual_spi(struct vgic400 *vgic,
                                     uint16_t *interrupt_no, const char *name)
{
    errno_t ret;

    if (vgic->boolean.virtual_spi) {
        ret = allocate_virtual_spi(vgic, interrupt_no, name);
    } else {
        ret = -EPERM;
    }

    return ret;
}
