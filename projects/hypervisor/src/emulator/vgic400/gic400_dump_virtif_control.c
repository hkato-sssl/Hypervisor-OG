/*
 * gic400_dump_virtif_control.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/system/printk.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void gic400_dump_virtif_control(struct vgic400 *vgic)
{
    int i;

    printk("<GIC-400 Virtual interface control>\n");

    printk("   GICH_HCR: 0x%08x\n", gic400_read_virtif_control(vgic, GICH_HCR));
    printk("   GICH_VTR: 0x%08x\n", gic400_read_virtif_control(vgic, GICH_VTR));
    printk("  GICH_VMCR: 0x%08x\n",
           gic400_read_virtif_control(vgic, GICH_VMCR));
    printk("  GICH_MISR: 0x%08x\n",
           gic400_read_virtif_control(vgic, GICH_MISR));
    printk(" GICH_EISR0: 0x%08x\n",
           gic400_read_virtif_control(vgic, GICH_EISR0));
    printk("GICH_ELRSR0: 0x%08x\n",
           gic400_read_virtif_control(vgic, GICH_ELRSR0));
    printk("  GICH_APR0: 0x%08x\n",
           gic400_read_virtif_control(vgic, GICH_APR0));
    for (i = 0; i < 4; ++i) {
        printk("   GICH_LR%d: 0x%08x\n", i,
               gic400_read_virtif_control(vgic, GICH_LR(i)));
    }
    printk("\n");
}
