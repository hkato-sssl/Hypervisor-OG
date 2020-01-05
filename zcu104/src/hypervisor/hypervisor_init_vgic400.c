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
    trap.emulator.handler = (vpc_memory_access_emulator_t)vgic400_distributor_emulate_memory_access;
    ret = vm_register_region_trap(vm, &trap);

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

printk("<%s>\n", __func__);
    ret = configure(vm);
printk("configure() -> %d\n", ret);
    if (ret == SUCCESS) {
        ret = register_trap(vm);
printk("register_trap() -> %d\n", ret);
    }

    return ret;
}

