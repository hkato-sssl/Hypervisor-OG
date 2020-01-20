/*
 * emulator/vgic400/vgic400_configure_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "driver/arm/gic400.h"
#include "hypervisor/emulator/vgic400.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config)
{
    uint32_t i;
    uint32_t d;

    d = 1 << 28;        /* State = pending */
    if (config->flag.hw != 0) {
        d |= BIT(31);   /* HW */
    }
    if (config->flag.group1 != 0) {
        d |= BIT(30);   /* Grp1 */
    }
    d |= (uint32_t)(config->physical_id) << 10;
    d |= (uint32_t)(config->virtual_id);

    if (config->virtual_id < 32) {
        for (i = 0; i < MAX_NR_VM_PROCESSORS; ++i) {
            vgic->template.ppi[i][config->virtual_id - 16] = d;
        }
    } else {
        vgic->template.spi[config->virtual_id - 32] = d;
    }

    return SUCCESS;
}

static bool is_valid_id(uint16_t id)
{
    bool valid;

    if ((id != GIC400_MAINTENANCE_INTERRUPT) && (id != GIC400_HYPERVISOR_TIMER) && (id < NR_GIC400_INTERRUPTS)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

static bool is_valid_configuration(const struct vgic400_interrupt_configuration *config)
{
    bool valid;

    if (is_valid_id(config->virtual_id) && is_valid_id(config->physical_id)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t vgic400_configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config)
{
    errno_t ret;

    if (is_valid_configuration(config)) {
        ret = configure_interrupt(vgic, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

