/*
 * emulator/vgic400emulator/vgic400/vgic400_assert_virtual_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "driver/system/cpu.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t inject_virtual_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;
    uint8_t targets;
    uint32_t d;
    uint32_t proc;
    uint32_t current_proc;

    d = (uint32_t)(vgic->virtual_spi.priorityr[interrupt_no - vgic->virtual_spi.base_no]) << 23;
    d |= BIT(28) | BIT(19) | interrupt_no;   /* pending, EOI, VirtualID */

    current_proc = cpu_no();
    proc = vm_physical_proc_no(vgic->vm, 0);
    if (current_proc == proc) {
        gic400_write_virtif_control(vgic, GICH_LR(0), d);
        ret = SUCCESS;
    } else {
        vgic->lr[0][0] = d;
        vgic->iar[0][0] = interrupt_no;
        targets = 1 << proc;
        ret = gic400_assert_sgi(vgic->gic, targets, 7);
    }

    return ret;
}

static errno_t assert_virtual_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;
    uint32_t no;

    vgic400_lock(vgic);

    no = interrupt_no - vgic->virtual_spi.base_no;
    if ((vgic->virtual_spi.priorityr[no] != vgic->priority_mask) && (vgic->lr[0][0] == 0) && (vgic->iar[0][0] == 0)) {
        ret = inject_virtual_interrupt(vgic, interrupt_no);
    } else {
        vgic->virtual_spi.pendr |= 1UL << no;
        ret = SUCCESS;
    }

    vgic400_unlock(vgic);

    return ret;
}

static errno_t validate_parameters(struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;
    uint32_t base;

    if (vgic->boolean.virtual_spi) {
        base = vgic->virtual_spi.base_no;
        if ((interrupt_no >= base) && (interrupt_no < (base + 32))) {
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EPERM;
    }

    return ret;
}

errno_t vgic400_assert_virtual_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;

    ret = validate_parameters(vgic, interrupt_no);
    if (ret == SUCCESS) {
        if (vgic->iar[0][0] != interrupt_no) {
            ret = assert_virtual_interrupt(vgic, interrupt_no);
        } else {
            /* Already the interrupt has injected. */
            ret = SUCCESS;
        }
    }

    return ret;
}

