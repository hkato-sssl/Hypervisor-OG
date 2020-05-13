/*
 * emulator/vgic400/vgic400_configure_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config)
{
    uint32_t d;
    uint32_t i;
    uint32_t idx;

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
        idx = config->virtual_id - 16;
        for (i =0; i < vgic->vm->nr_procs; ++i) {
            vgic->ppi[i].template[idx] = d;
        }
    } else {
        vgic->spi.template[config->virtual_id - 32] = d;
    }

    return SUCCESS;
}

static bool is_valid_id(uint16_t id)
{
    bool ret;

    if ((id != GIC400_MAINTENANCE_INTERRUPT) && (id != GIC400_HYPERVISOR_TIMER) && (id >= 16) && (id < NR_GIC400_INTERRUPTS)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static errno_t validate_configuration(const struct vgic400_interrupt_configuration *config)
{
    errno_t ret;

    if (! is_valid_id(config->virtual_id)) {
        ret = -EINVAL;
    } else if (! is_valid_id(config->physical_id)) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t vgic400_configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config)
{
    errno_t ret;

    ret = validate_configuration(config);
    if (ret == SUCCESS) {
        ret = configure_interrupt(vgic, config);
    }

    return ret;
}

