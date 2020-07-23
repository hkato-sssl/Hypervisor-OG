/*
 * emulator/vgic400/vgic400_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/mmu.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

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
    ret = aarch64_stage2_map(&(vgic->vm->stage2), addr, vgic->base.virtual_cpuif, 4096, &attr);

    return ret;
}

static errno_t register_trap_cpuif(struct vgic400 *vgic)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = &(vgic->trap.cpuif);
    trap->condition.read = false;
    trap->condition.write = true;
    trap->memory.ipa = (uint64_t)gic400_cpuif_register_base(vgic->gic);
    trap->memory.pa = (uint64_t)vgic->base.virtual_cpuif;
    trap->memory.size = 4096;
    trap->memory.shareability = HYP_MMU_SH_NSH;
    trap->memory.type = HYP_MMU_MT_DEVICE_nGnRE;
    trap->emulator.arg = vgic;
    trap->emulator.handler = (vpc_emulator_t)vgic400_cpuif_emulate_memory_access;
    ret = vm_register_region_trap(vgic->vm, trap);

    return ret;
}

static errno_t register_trap_distributor(struct vgic400 *vgic, const struct vgic400_configuration *config)
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
    trap->emulator.handler = (vpc_emulator_t)vgic400_distributor_emulate_memory_access;
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

static errno_t initialize_virtual_spi(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;
    int i;

    if ((vgic->template_typer & BITS(4, 0)) < 0x1f) {
        ++(vgic->template_typer);   /* Increment GICD_TYPER.ITLinesNumber */
        vgic->virtual_spi.base_no = (vgic->template_typer & BITS(4, 0)) * 32;
        for (i = 0; i < 32; ++i) {
            vgic->virtual_spi.priorityr[i] = vgic->priority_mask;
        }
        vgic->virtual_spi.used = 0;
        ret = SUCCESS;
    } else {
        ret = -EPERM;
    }

    return ret;
}

static errno_t initialize(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;
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

    /* probe the # of List Register */

    d = gic400_read_virtif_control(vgic, GICH_VTR);
    vgic->nr_list_registers = BF_EXTRACT(d, 5, 0) + 1;

    /* probe GIC distributor */

    d = gic400_read_distributor(vgic->gic, GICD_TYPER);
    vgic->template_typer = d & ~(uint32_t)BITS(7, 5);

    vgic->boolean.ignore_priority0 = config->boolean.ignore_priority0;
    vgic->boolean.virtual_spi = config->boolean.virtual_spi;
    if (vgic->boolean.virtual_spi) {
        initialize_virtual_spi(vgic, config);
    }

    ret = register_trap_distributor(vgic, config);

    return ret;
}

errno_t vgic400_initialize(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;

    if (config->ops == NULL) {
        ret = -EINVAL;
    } else {
        ret = initialize(vgic, config);
    }

    return ret;
}

