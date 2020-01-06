/*
 * hypervisor/hypervisor_init_vgic400.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern struct gic400 gic;
static struct vgic400 vgic;
static struct vm_region_trap trap;

/* functions */

static errno_t register_trap(struct vm *vm)
{
    errno_t ret;

    memset(&trap, 0, sizeof(trap));
    trap.ipa.addr = CONFIG_GICD_BASE;
    trap.ipa.size = 4096;
    trap.emulator.arg = &vgic;
    trap.emulator.handler = (vpc_emulator_t)vgic400_distributor_emulate_memory_access;
    ret = vm_register_region_trap(vm, &trap);

    return ret;
}

static errno_t map_cpuif(struct vm *vm)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 1;
    attr.af = 1;
    attr.sh = STAGE2_SH_OSH;
    attr.s2ap = STAGE2_S2AP_RW;
    attr.memattr = STAGE2_MEMATTR_DEVICE_GRE;
    ret = aarch64_stage2_map(vm->stage2, (void *)CONFIG_GICC_BASE, (void *)CONFIG_GICV_BASE, 4096, &attr);

    return ret;
}

static errno_t configure(struct vm *vm)
{
    errno_t ret;
    struct vgic400_configuration config;

    memset(&config, 0, sizeof(config));
    config.owner = vm;
    config.gic = &gic;
    config.irq.num = 0;
    config.irq.array = NULL;
    ret = vgic400_configure(&vgic, &config);

    return ret;
}

errno_t hypervisor_init_vgic400(struct vm *vm)
{
    errno_t ret;

    ret = configure(vm);

    if (ret == SUCCESS) {
        ret = map_cpuif(vm);
    }

    if (ret == SUCCESS) {
        ret = register_trap(vm);
    }

    return ret;
}

