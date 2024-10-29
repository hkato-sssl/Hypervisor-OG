// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_update_list_register.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_update_list_registers(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;
    int no;
    struct vgic400_interrupt_event event;

    vgic400_lock(vgic);

    do {
        no = vgic400_list_register(vgic);
        if (no >= 0) {
            ret = vgic400_pop_interrupt_event(vpc, vgic, &event);
            if (ret == SUCCESS) {
                ret = vgic400_write_interrupt_event(vpc, vgic, &event, no);
            }
        } else {
            ret = SUCCESS; /* all List Regisers are used. */
            break;
        }
    } while (ret == SUCCESS);

    vgic400_unlock(vgic);

    return ret;
}