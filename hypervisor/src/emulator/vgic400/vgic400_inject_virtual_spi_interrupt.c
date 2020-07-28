/*
 * emulator/vgic400emulator/vgic400/vgic400_virtual_spi_inject_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

/* CAUTION:
 * 本APIは内部から呼び出される事を前提としている為、排他制御を実
 * 装していない。
 * 本APIを呼び出す場合は必ずvgic400_lock()を実行している状態で呼
 * び出す必要がある。
 */

errno_t vgic400_inject_virtual_spi_interrupt(struct vgic400 *vgic, uint16_t interrupt_no)
{
    uint32_t d;

    d = (uint32_t)(vgic->virtual_spi.ipriorityr[interrupt_no - vgic->virtual_spi.base_no]) << 23;
    d |= BIT(28) | BIT(19) | interrupt_no;   /* pending, EOI, VirtualID */
    gic400_write_virtif_control(vgic, GICH_LR(0), d);

    vgic->virtual_spi.asserting = true;

    return SUCCESS;
}

