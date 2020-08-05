/*
 * emulator/vgic400emulator/vgic400/vgic400_expose_virtual_spi_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
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

errno_t inject_virtual_spi_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
{
    uint16_t virtual_id;
    uint32_t d;

    virtual_id = interrupt_no + vgic->virtual_spi.base_no;
    d = (uint32_t)(vgic->virtual_spi.ipriorityr[interrupt_no]) << 23;
    d |= BIT(28) | BIT(19) | virtual_id;    /* pending, EOI, VirtualID */
    gic400_write_virtif_control(vgic, GICH_LR(0), d);

    vgic->virtual_spi.ipendr &= ~(uint32_t)BIT(interrupt_no);

    return SUCCESS;
}

static errno_t discontinue_virtual_spi_interrupt(struct vgic400 *vgic)
{
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_HCR);
    d &= ~(uint32_t)BIT(3);     /* NPIE: No Pending Interrupt Enable. */
    gic400_write_virtif_control(vgic, GICH_HCR, d);

    vgic->virtual_spi.asserting = false;

    return SUCCESS;
}

static errno_t expose_virtual_spi_interrupt(struct vgic400 *vgic)
{
    errno_t ret;
    uint8_t priority;
    uint32_t no;
    uint32_t interrupt_no;
    uint64_t d;

    priority = vgic->priority_mask;
    interrupt_no = 0;

    d = vgic->virtual_spi.ipendr & vgic->virtual_spi.ienabler;
    d = aarch64_rbit(d);
    while (d != 0) {
        no = (uint32_t)aarch64_clz(d);
        if (vgic->virtual_spi.ipriorityr[no] < priority) {
            priority = vgic->virtual_spi.ipriorityr[no];
            interrupt_no = no;
        }
        d ^= BIT(63 - no);
    }

    if (priority < vgic->priority_mask) {
        ret = inject_virtual_spi_interrupt(vgic, interrupt_no);
    } else {
        /* No exposable interrupt is pending. */
        ret = discontinue_virtual_spi_interrupt(vgic);
    }

    return ret;
}

errno_t vgic400_expose_virtual_spi_interrupt(struct vpc *vpc, struct vgic400 *vgic)
{
    errno_t ret;

    vgic400_lock(vgic);

    if ((vgic->boolean.virtual_spi) && (vpc->proc_no == 0)) {
        ret = expose_virtual_spi_interrupt(vgic);
    } else {
        ret = -EPERM;
    }

    vgic400_unlock(vgic);

    return ret;
}

