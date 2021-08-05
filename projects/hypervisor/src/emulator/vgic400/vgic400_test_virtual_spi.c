/*
 * emulator/vgic400/vgic400_test_virtual_spi.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/emulator/vgic400.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

bool vgic400_test_virtual_spi(struct vgic400 *vgic, uint16_t interrupt_no)
{
    bool result;

    if (vgic->boolean.virtual_spi) {
        if ((vgic->virtual_spi.base_no <= interrupt_no)
            && ((vgic->virtual_spi.base_no + 32) > interrupt_no)) {
            result = true;
        } else {
            result = false;
        }
    } else {
        result = false;
    }

    return result;
}
