/*
 * emulator/vgic400/vgic400_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "driver/aarch64/stage2.h"
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
    attr.memattr = STAGE2_MEMATTR_DEVICE_GRE;
    attr.s2ap = STAGE2_S2AP_RW;

    addr = gic400_cpuif_register_base(vgic->gic);
    ret = aarch64_stage2_map(vgic->vm->stage2, addr, vgic->base.virtual_cpuif, 4096, &attr);

    return ret;
}

static errno_t register_trap_cpuif(struct vgic400 *vgic)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = &(vgic->trap.distributor);
    trap->condition.read = false;
    trap->condition.write = true;
    trap->ipa = (uint64_t)gic400_cpuif_register_base(vgic->gic);
    trap->pa = (uint64_t)vgic->base.virtual_cpuif;
    trap->size = 4096;
    trap->emulator.arg = &vgic;
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
    trap->ipa = (uint64_t)gic400_distributor_register_base(vgic->gic);
    trap->pa = trap->ipa;
    trap->size = 4096;
    trap->emulator.arg = &vgic;
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

static errno_t initialize(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;
    uint32_t d;

    memset(vgic, 0, sizeof(*vgic));
    vgic->vm = config->vm;
    vgic->gic = config->gic;
    vgic->base.virtif_control = config->base.virtif_control;
    vgic->base.virtual_cpuif = config->base.virtual_cpuif;
    vgic->priority_mask = gic400_priority_mask(vgic->gic);

    /* probe # of List Register */

    d = gic400_read_virtif_control(vgic, GICH_VTR);
    vgic->nr_list_registers = BF_EXTRACT(d, 5, 0) + 1;

    /* initialize the target map */

    /*   ID  Assign      Source            
     *   31  guest       Legacy IRQ signal              
     *   30  guest       Non-secure Physical timer 
     *   29  N/A         Secure physical timer
     *   28  guest       Legacy FIQ signal
     *   27  guest       Virtual timer
     *   26  hypervisor  Hypervisor timer
     *   25  hypervisor  Virtual maintenance interrupt
     * 24-16 guest       Undefined
     */

    vgic->target.irq[0] = 0xd90000ff;

    ret = register_trap_distributor(vgic, config);

    return ret;
}

errno_t vgic400_initialize(struct vgic400 *vgic, const struct vgic400_configuration *config)
{
    errno_t ret;

    if ((vgic != NULL) && (config != NULL)) {
        ret = initialize(vgic, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

