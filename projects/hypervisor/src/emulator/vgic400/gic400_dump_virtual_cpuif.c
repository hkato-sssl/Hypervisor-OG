/*
 * gic400_dump_virtual_cpuif.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "driver/system/cpu.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/system/printk.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

#define NR_VCPUIFS 8

#define VCPUIF_REG_BASE(n)  (0x5000 + (n) * 0x200)

/* types */

/* prototypes */

/* variables */

/* functions */

void gic400_dump_virtual_cpuif_at(struct vgic400 *vgic, uint8_t no)
{
    int i;

    if (no >= NR_VCPUIFS) {
        printk("%s: The value of no(=%u) is out of range.\n", __func__, no);
        return;
    }

    printk("<GIC-400 Virtual CPU interface#%u>\n", no);
    printk("  GICV_CTLR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_CTLR));
    printk("   GICV_PMR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_PMR));
    printk("   GICV_BPR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_BPR));
    printk("   GICV_IAR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_IAR));
    printk("   GICV_RPR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_RPR));
    printk(" GICV_HPPIR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_HPPIR));
    printk("  GICV_ABPR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_ABPR));
    printk("  GICV_AIAR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_AIAR));
    printk("GICV_AHPPIR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_AHPPIR));
    printk("  GICV_APR0: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_APR0));
    printk("  GICV_IIDR: 0x%08x\n", gic400_read_virtual_cpuif(vgic, GICV_IIDR));
    printk("\n");
}

void gic400_dump_virtual_cpuif(struct vgic400 *vgic)
{
    gic400_dump_virtual_cpuif_at(vgic, cpu_no());
}