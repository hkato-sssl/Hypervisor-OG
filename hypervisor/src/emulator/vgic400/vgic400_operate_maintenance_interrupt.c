/*
 * emulator/vgic400/vgic400_operate_maintenance_interrupt.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/aarch64.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void maintenance_misr_eoi(struct vpc *vpc, struct vgic400 *vgic)
{
    uint32_t d;
    uint32_t no;
    uint32_t *iar;

    iar = vgic->iar[vpc->proc_no];
    d = gic400_read_virtif_control(vgic, GICH_EISR0);
    while (d != 0) {
        no = 63 - (uint32_t)aarch64_clz(d);
        gic400_deactivate(vgic->gic, iar[no]);
        gic400_write_virtif_control(vgic, GICH_LR(no), 0);
        d ^= BIT(no);
    }
}

static errno_t maintenance_interrupt(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;
    uint32_t d;

    d = gic400_read_virtif_control(vgic, GICH_MISR);

    if ((d & BIT(0)) != 0) {    /* EOI */
        maintenance_misr_eoi(vpc, vgic);
        d = gic400_read_virtif_control(vgic, GICH_MISR);
    }

    ret = gic400_deactivate(vgic->gic, iar);

    return ret;
}

errno_t vgic400_operate_maintenance_interrupt(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar)
{
    errno_t ret;

    /* 実装時では必要無しと判断して排他制御は実装していない。*/

    ret = maintenance_interrupt(vpc, vgic, iar);

    return ret;
}

