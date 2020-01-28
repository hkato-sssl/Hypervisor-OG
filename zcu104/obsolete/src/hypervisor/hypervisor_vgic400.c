/*
 * hypervisor/hypervisor_vgic400.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/aarch64/stage2.h"
#include "driver/xilinx/mpsoc.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern struct gic400 sys_gic;
static struct vgic400 vgic;
static struct vm_region_trap trap_c;
static struct vm_region_trap trap_d;

/* functions */

errno_t hypervisor_emulate_vgic400_irq(struct vpc *vpc)
{
    errno_t ret;

    ret = vgic400_emulate_irq_exception(&vgic, vpc);

    return ret;
}

static errno_t register_trap_cpuif(struct vm *vm)
{
    errno_t ret;

    memset(&trap_c, 0, sizeof(trap_c));
    trap_c.condition.read = false;
    trap_c.condition.write = true;
    trap_c.ipa = GIC400C_BASE;
    trap_c.pa = GIC400V_BASE;
    trap_c.size = 4096;
    trap_c.emulator.arg = &vgic;
    trap_c.emulator.handler = (vpc_emulator_t)vgic400_cpuif_emulate_memory_access;
    ret = vm_register_region_trap(vm, &trap_c);

    return ret;
}

static errno_t register_trap_distributor(struct vm *vm)
{
    errno_t ret;

    memset(&trap_d, 0, sizeof(trap_d));
    trap_d.condition.read = true;
    trap_d.condition.write = true;
    trap_d.ipa = GIC400D_BASE;
    trap_d.pa = GIC400D_BASE;
    trap_d.size = 4096;
    trap_d.emulator.arg = &vgic;
    trap_d.emulator.handler = (vpc_emulator_t)vgic400_distributor_emulate_memory_access;
    ret = vm_register_region_trap(vm, &trap_d);

    return ret;
}

static errno_t configure(struct vm *vm)
{
    errno_t ret;
    struct vgic400_configuration config;

    memset(&config, 0, sizeof(config));
    config.owner = vm;
    config.gic = &sys_gic;
    config.base.virtif_control = (void *)GIC400H_BASE;
    config.base.virtual_cpuif = (void *)GIC400V_BASE;
    ret = vgic400_configure(&vgic, &config);

    return ret;
}

errno_t hypervisor_init_vgic400(struct vm *vm)
{
    errno_t ret;

    ret = configure(vm);

    if (ret == SUCCESS) {
        ret = register_trap_cpuif(vm);
    }

    if (ret == SUCCESS) {
        ret = register_trap_distributor(vm);
    }

    if (ret == SUCCESS) {
        ret = vgic400_activate(&vgic);
    }

    return ret;
}

