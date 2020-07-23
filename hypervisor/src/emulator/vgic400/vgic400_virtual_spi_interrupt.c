/*
 * emulator/vgic400emulator/vgic400/vgic400_virtual_spi_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/aarch64.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "driver/system/cpu.h"
#include "hypervisor/parameter.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t inject_virtual_spi_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
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
        ret = gic400_assert_sgi(vgic->gic, targets, HYP_INTR_REQUEST_VIRTUAL_SPI);
    }

    return ret;
}

static errno_t assert_virtual_spi_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;
    uint32_t no;

    vgic400_lock(vgic);

    no = interrupt_no - vgic->virtual_spi.base_no;
    if ((vgic->virtual_spi.priorityr[no] != vgic->priority_mask) && (vgic->lr[0][0] == 0) && (vgic->iar[0][0] == 0)) {
        ret = inject_virtual_spi_interrupt(vgic, interrupt_no);
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

errno_t vgic400_assert_virtual_spi_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
{
    errno_t ret;

    ret = validate_parameters(vgic, interrupt_no);
    if (ret == SUCCESS) {
        ret = assert_virtual_spi_interrupt(vgic, interrupt_no);
    }

    return ret;
}

static errno_t encourage_virtual_spi_interrupt(struct vgic400 *vgic)
{
    errno_t ret;
    uint8_t priority;
    uint16_t no;
    uint16_t interrupt_no;
    uint32_t d;

    priority = vgic->priority_mask;

    d = vgic->virtual_spi.pendr;
    while (d != 0) {
        no = 63 - (uint16_t)aarch64_clz(d);
        if (vgic->virtual_spi.priorityr[no] < priority) {
            priority = vgic->virtual_spi.priorityr[no];
            interrupt_no = no;
        }
        d &= ~(uint32_t)BIT(no);
    }

    if (priority < vgic->priority_mask) {
        ret = inject_virtual_spi_interrupt(vgic, (interrupt_no + vgic->virtual_spi.base_no));
        if (ret == SUCCESS) {
            vgic->virtual_spi.pendr &= ~(uint32_t)BIT(interrupt_no);
        }
    } else {
        /* No assertable interrupt is pending. */

        ret = SUCCESS;
    }

    return ret;
}

errno_t vgic400_encourage_virtual_spi_interrupt(struct vgic400 *vgic)
{
    errno_t ret;

    vgic400_lock(vgic);

    if ((vgic->virtual_spi.pendr != 0) && (vgic->lr[0][0] == 0) && (vgic->iar[0][0] == 0)) {
        ret = encourage_virtual_spi_interrupt(vgic);
    } else {
        ret = SUCCESS;  /* no work */
    }

    vgic400_unlock(vgic);

    return ret;
}

