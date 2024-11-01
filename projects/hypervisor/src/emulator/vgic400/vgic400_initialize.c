// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/stage2.h"
#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/mmu.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "vgic400_local.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t map_cpuif(struct vgic400 *vgic)
{
    errno_t ret;
    void *addr;
    struct aarch64_stage2_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 1;
    attr.af = 1;
    attr.sh = STAGE2_SH_NSH;
    attr.memattr = STAGE2_MEMATTR_DEVICE_nGnRE;
    attr.s2ap = STAGE2_S2AP_RW;

    addr = gic400_cpuif_register_base(vgic->gic);
    ret = aarch64_stage2_map(&(vgic->vm->stage2), addr,
                             vgic->base.virtual_cpuif, 4096, &attr);

    return ret;
}

static errno_t register_trap_cpuif(struct vgic400 *vgic)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = &(vgic->trap.cpuif[0]);
    trap->condition.read = false;
    trap->condition.write = true;
    trap->memory.ipa = (uint64_t)gic400_cpuif_register_base(vgic->gic);
    trap->memory.pa = (uint64_t)vgic->base.virtual_cpuif;
    trap->memory.size = 0x1000;
    trap->memory.shareability = HYP_MMU_SH_NSH;
    trap->memory.type = HYP_MMU_MT_DEVICE_nGnRE;
    trap->emulator.arg = vgic;
    trap->emulator.handler =
        (vpc_emulator_t)vgic400_cpuif_emulate_memory_access;

    vgic->trap.cpuif[1] = *trap;
    vgic->trap.cpuif[1].memory.ipa += GICC_DIR;
    vgic->trap.cpuif[1].memory.pa += GICV_DIR;

    ret = vm_register_region_trap(vgic->vm, trap);
    if (ret == SUCCESS) {
        ret = vm_register_region_trap(vgic->vm, &(vgic->trap.cpuif[1]));
    }

    return ret;
}

static errno_t
register_trap_distributor(struct vgic400 *vgic,
                          const struct vgic400_configuration *config)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = &(vgic->trap.distributor);
    trap->condition.read = true;
    trap->condition.write = true;
    trap->memory.ipa = (uint64_t)gic400_distributor_register_base(vgic->gic);
    trap->memory.pa = trap->memory.ipa;
    trap->memory.size = 4096;
    trap->memory.shareability = HYP_MMU_SH_OSH;
    trap->memory.type = HYP_MMU_MT_DEVICE_nGnRE;
    trap->emulator.arg = vgic;
    trap->emulator.handler =
        (vpc_emulator_t)vgic400_distributor_emulate_memory_access;
    ret = vm_register_region_trap(vgic->vm, trap);
    if (ret == SUCCESS) {
        if (config->boolean.trap_cpuif) {
            ret = register_trap_cpuif(vgic);
        } else {
            ret = map_cpuif(vgic);
        }
    }

    return ret;
}

static errno_t initialize(struct vgic400 *vgic,
                          const struct vgic400_configuration *config)
{
    errno_t ret;
    int i;
    uint32_t d;

    memset(vgic, 0, sizeof(*vgic));
    memset(&(vgic->spi.map), 0xff, sizeof(vgic->spi.map));

    spin_lock_init(&(vgic->lock));

    vgic->vm = config->vm;
    vgic->gic = config->gic;
    vgic->base.virtif_control = config->base.virtif_control;
    vgic->base.virtual_cpuif = config->base.virtual_cpuif;
    vgic->priority_mask = gic400_priority_mask(vgic->gic);
    vgic->ops = config->ops;

    for (i = 0; i < NR_VGIC400_CPUS; ++i) {
        vgic->interrupt.event_arrays[i] = config->event_arrays[i];
    }

    /* probe the # of List Register */

    d = gic400_read_virtif_control(vgic, GICH_VTR);
    vgic->nr_list_registers = BF_EXTRACT(d, 5, 0) + 1;

    /* probe GIC distributor */

    d = gic400_read_distributor(vgic->gic, GICD_TYPER);
    vgic->template_typer = d & ~(uint32_t)BITS(7, 5);

    vgic->boolean.half_priority = config->boolean.half_priority;
    if (vgic->boolean.half_priority) {
        vgic->priority_mask <<= 1;
    }

    ret = register_trap_distributor(vgic, config);

    return ret;
}

static bool validate_event_arrays(const struct vgic400_configuration *config)
{
    bool result;
    int i;

    result = true;

    for (i = 0; i < config->vm->nr_procs; ++i) {
        if (config->event_arrays[i] == NULL) {
            result = false;
            break;
        }
    }

    return result;
}

static bool validate_parameters(const struct vgic400_configuration *config)
{
    bool result;

    if ((config != NULL) && (config->vm != NULL) && (config->gic != NULL)
        && (config->base.virtif_control != NULL)
        && (config->base.virtual_cpuif != NULL) && (config->ops != NULL)) {
        result = validate_event_arrays(config);
    } else {
        result = false;
    }

    return result;
}

errno_t vgic400_initialize(struct vgic400 *vgic,
                           const struct vgic400_configuration *config)
{
    errno_t ret;

    if (validate_parameters(config)) {
        ret = initialize(vgic, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
