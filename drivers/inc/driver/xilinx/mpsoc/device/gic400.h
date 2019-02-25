/*
 * driver/xilinx/mpsoc/device/gic400.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_XILINX_MPSOC_DEVICE_GIC400_H
#define DRIVER_XILINX_MPSOC_DEVICE_GIC400_H

/* includes */

#include "lib/bit.h"

/* defines */

/* GIC distributor */

#define GICD_CTLR           0x00010000
#define GICD_TYPER          0x00010004
#define GICD_IDDR           0x00010008
#define GICD_IGROUPR(n)     (0x00010080 + (n) * 4)
#define GICD_ISENABLER(n)   (0x00010100 + (n) * 4)
#define GICD_ICENABLER(n)   (0x00010180 + (n) * 4)
#define GICD_ISPENDR(n)     (0x00010200 + (n) * 4)
#define GICD_ICPENDR(n)     (0x00010280 + (n) * 4)
#define GICD_ISACTIVER(n)   (0x00010300 + (n) * 4)
#define GICD_ICACTIVER(n)   (0x00010380 + (n) * 4)
#define GICD_IPRIORITYR(n)  (0x00010400 + (n) * 4)
#define GICD_ITARGETSR(n)   (0x00010800 + (n) * 4)
#define GICD_ICFGR(n)       (0x00010c00 + (n) * 4)
#define GICD_PPISR          0x00010d00
#define GICD_SPISR(n)       (0x00010d04 + (n) * 4)
#define GICD_SGIR           0x00010f00
#define GICD_CPENDSGIR(n)   (0x00010f10 + (n) * 4)
#define GICD_SPENDSGIR(n)   (0x00010f20 + (n) * 4)
#define GICD_PIDR4          0x00010fd0
#define GICD_PIDR5          0x00010fd4
#define GICD_PIDR6          0x00010fd8
#define GICD_PIDR7          0x00010fdc
#define GICD_PIDR0          0x00010fe0
#define GICD_PIDR1          0x00010fe4
#define GICD_PIDR2          0x00010fe8
#define GICD_PIDR3          0x00010fec
#define GICD_CIDR(n)        (0x00010ff0 + (n) * 4)

/* GIC cpu interface */

#define GICC_CTLR           0x00020000
#define GICC_PMR            0x00020004
#define GICC_BPR            0x00020008
#define GICC_IAR            0x0002000c
#define GICC_EOIR           0x00020010
#define GICC_RPR            0x00020014
#define GICC_HPPIR          0x00020018
#define GICC_ABPR           0x0002001c
#define GICC_AIAR           0x00020020
#define GICC_AEOIR          0x00020024
#define GICC_AHPPIR         0x00020028
#define GICC_APR0           0x000200d0
#define GICC_NSAPR0         0x000200e0
#define GICC_IIDR           0x000200fc
#define GICC_DIR            0x00030000
#define GICC_HCR            0x00040000
#define GICC_VTR            0x00040004
#define GICC_VMCR           0x00040008
#define GICC_MISR           0x00040010
#define GICC_EISR0          0x00040020
#define GICC_ELSR0          0x00040030

#define GICH_APR0           0x000400f0
#define GICH_LR(n)          (0x00040100 + (n) * 4)

#define GICH_HCR_ALIAS(n)   (0x00050000 + (n) * 0x0200)
#define GICH_VTR_ALIAS(n)   (0x00050004 + (n) * 0x0200)
#define GICH_VMCR_ALIAS(n)  (0x00050008 + (n) * 0x0200)
#define GICH_MISR_ALIAS(n)  (0x00050010 + (n) * 0x0200)
#define GICH_EISR0_ALIAS(n) (0x00050020 + (n) * 0x0200)
#define GICH_ELSR0_ALIAS(n) (0x00050030 + (n) * 0x0200)
#define GICH_APR0_ALIAS(n)  (0x000500f0 + (n) * 0x0200)
#define GICH_LR0_ALIAS(n)   (0x00050100 + (n) * 0x0200)
#define GICH_LR1_ALIAS(n)   (0x00050104 + (n) * 0x0200)
#define GICH_LR2_ALIAS(n)   (0x00050108 + (n) * 0x0200)
#define GICH_LR3_ALIAS(n)   (0x0005010c + (n) * 0x0200)

#define GICV_CTLR           0x00060000
#define GICV_PMR            0x00060004
#define GICV_BPR            0x00060008
#define GICV_IAR            0x0006000c
#define GICV_EOIR           0x00060010
#define GICV_RPR            0x00060014
#define GICV_HPPIR          0x00060018
#define GICV_ABPR           0x0006001c
#define GICV_AIAR           0x00060020
#define GICV_AEOIR          0x00060024
#define GICV_AHPPIR         0x00060028
#define GICV_APR0           0x000600d0
#define GICV_IIDR           0x000600fc
#define GICV_DIR            0x00070000

#endif /* DRIVER_XILINX_MPSOC_DEVICE_GIC400_H */
