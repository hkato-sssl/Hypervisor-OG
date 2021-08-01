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
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void set_target_bit(uint32_t *targets, uint32_t no)
{
    targets[no / 32] |= 1UL << (no % 32);
}

static errno_t configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config)
{
    uint32_t d;
    uint32_t i;
    uint32_t idx;

    d = BIT(28);    /* State = pending */
    if (config->flag.hw != 0) {
        d |= BIT(31);   /* HW */
    }
    if (config->flag.group1 != 0) {
        d |= BIT(30);   /* Grp1 */
    }
    d |= (uint32_t)(vgic->priority_mask) << 20;
    d |= (uint32_t)(config->physical_id) << 10;
    d |= (uint32_t)(config->virtual_id);

    if ((config->virtual_id >= 16) && (config->virtual_id < 32)) {
        idx = config->virtual_id - 16;
        for (i =0; i < vgic->vm->nr_procs; ++i) {
            vgic->ppi[i].template[idx] = d;
        }
    } else if (config->virtual_id >= 32) {
        vgic->spi.template[config->virtual_id - 32] = d;
        vgic->spi.map.virtual[config->physical_id - 32] = config->virtual_id;
        vgic->spi.map.physical[config->virtual_id - 32] = config->physical_id;
    }

    set_target_bit(vgic->target.virq, config->virtual_id);
    set_target_bit(vgic->target.irq, config->physical_id);

    return SUCCESS;
}

static bool is_valid_id(const struct vgic400 *vgic, uint16_t id)
{
    bool ret;

    if ((id == 7) || (id == GIC400_SECURE_PHYSICAL_TIMER) || (id == GIC400_MAINTENANCE_INTERRUPT) || (id == GIC400_HYPERVISOR_TIMER) || (id >= NR_GIC400_INTERRUPTS)) {
        ret = false;
    } else if (vgic->boolean.virtual_spi) {
        if ((id >= vgic->virtual_spi.base_no) && (id <= (vgic->virtual_spi.base_no + 31))) {
            ret = false;
        } else {
            ret = true;
        }
    } else {
        ret = true;
    }

    return ret;
}

static errno_t validate_configuration(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config)
{
    errno_t ret;

    if (! is_valid_id(vgic, config->virtual_id)) {
        ret = -EINVAL;
    } else if (! is_valid_id(vgic, config->physical_id)) {
        ret = -EINVAL;
    } else if ((config->virtual_id < 32) || (config->physical_id < 32)) {
        if (config->virtual_id != config->physical_id) {
            ret = -EINVAL;
        } else {
            ret = SUCCESS;
        }
    } else {
        if (vgic->spi.map.virtual[config->physical_id - 32] != VGIC400_NO_ASSIGNED) {
            ret = -EBUSY;
        } else if (vgic->spi.map.physical[config->virtual_id - 32] != VGIC400_NO_ASSIGNED) {
            ret = -EBUSY;
        } else {
            ret = SUCCESS;
        }
    }

    return ret;
}

errno_t vgic400_configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config)
{
    errno_t ret;

    ret = validate_configuration(vgic, config);
    if (ret == SUCCESS) {
        ret = configure_interrupt(vgic, config);
    }

    return ret;
}

