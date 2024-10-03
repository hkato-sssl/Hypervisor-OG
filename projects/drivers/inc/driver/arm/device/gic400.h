/*
 * driver/arm/device/gic400.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_GIC400_H
#define DRIVER_ARM_DEVICE_GIC400_H

/* includes */

/* defines */

/* GIC distributor */

#define GICD_CTLR          0x0000
#define GICD_TYPER         0x0004
#define GICD_IIDR          0x0008
#define GICD_IGROUPR(n)    (0x0080 + (n) * 4)
#define GICD_ISENABLER(n)  (0x0100 + (n) * 4)
#define GICD_ICENABLER(n)  (0x0180 + (n) * 4)
#define GICD_ISPENDR(n)    (0x0200 + (n) * 4)
#define GICD_ICPENDR(n)    (0x0280 + (n) * 4)
#define GICD_ISACTIVER(n)  (0x0300 + (n) * 4)
#define GICD_ICACTIVER(n)  (0x0380 + (n) * 4)
#define GICD_IPRIORITYR(n) (0x0400 + (n) * 4)
#define GICD_ITARGETSR(n)  (0x0800 + (n) * 4)
#define GICD_ICFGR(n)      (0x0c00 + (n) * 4)
#define GICD_PPISR         0x0d00
#define GICD_SPISR(n)      (0x0d04 + (n) * 4)
#define GICD_SGIR          0x0f00
#define GICD_CPENDSGIR(n)  (0x0f10 + (n) * 4)
#define GICD_SPENDSGIR(n)  (0x0f20 + (n) * 4)
#define GICD_PIDR4         0x0fd0
#define GICD_PIDR5         0x0fd4
#define GICD_PIDR6         0x0fd8
#define GICD_PIDR7         0x0fdc
#define GICD_PIDR0         0x0fe0
#define GICD_PIDR1         0x0fe4
#define GICD_PIDR2         0x0fe8
#define GICD_PIDR3         0x0fec
#define GICD_CIDR0         0x0ff0
#define GICD_CIDR1         0x0ff4
#define GICD_CIDR2         0x0ff8
#define GICD_CIDR3         0x0ffc

/* GIC cpu interface */

#define GICC_CTLR          0x0000
#define GICC_PMR           0x0004
#define GICC_BPR           0x0008
#define GICC_IAR           0x000c
#define GICC_EOIR          0x0010
#define GICC_RPR           0x0014
#define GICC_HPPIR         0x0018
#define GICC_ABPR          0x001c
#define GICC_AIAR          0x0020
#define GICC_AEOIR         0x0024
#define GICC_AHPPIR        0x0028
#define GICC_APR0          0x00d0
#define GICC_NSAPR0        0x00e0
#define GICC_IIDR          0x00fc

#ifdef XILINX_MPSOC
#define GICC_DIR 0x10000
#else
#define GICC_DIR 0x1000
#endif

/* virtual interface cotrol */

#define GICH_HCR             0x0000
#define GICH_VTR             0x0004
#define GICH_VMCR            0x0008
#define GICH_MISR            0x0010
#define GICH_EISR0           0x0020
#define GICH_ELRSR0          0x0030
#define GICH_APR0            0x00f0
#define GICH_LR(n)           (0x0100 + (n) * 4)

#define GICH_HCR_ALIAS(n)    (0x0000 + (n) * 0x0200)
#define GICH_VTR_ALIAS(n)    (0x0004 + (n) * 0x0200)
#define GICH_VMCR_ALIAS(n)   (0x0008 + (n) * 0x0200)
#define GICH_MISR_ALIAS(n)   (0x0010 + (n) * 0x0200)
#define GICH_EISR0_ALIAS(n)  (0x0020 + (n) * 0x0200)
#define GICH_ELRSR0_ALIAS(n) (0x0030 + (n) * 0x0200)
#define GICH_APR0_ALIAS(n)   (0x00f0 + (n) * 0x0200)
#define GICH_LR0_ALIAS(n)    (0x0100 + (n) * 0x0200)
#define GICH_LR1_ALIAS(n)    (0x0104 + (n) * 0x0200)
#define GICH_LR2_ALIAS(n)    (0x0108 + (n) * 0x0200)
#define GICH_LR3_ALIAS(n)    (0x010c + (n) * 0x0200)

#define GICV_CTLR            0x0000
#define GICV_PMR             0x0004
#define GICV_BPR             0x0008
#define GICV_IAR             0x000c
#define GICV_EOIR            0x0010
#define GICV_RPR             0x0014
#define GICV_HPPIR           0x0018
#define GICV_ABPR            0x001c
#define GICV_AIAR            0x0020
#define GICV_AEOIR           0x0024
#define GICV_AHPPIR          0x0028
#define GICV_APR0            0x00d0
#define GICV_IIDR            0x00fc
#define GICV_DIR             0x1000

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_DEVICE_GIC400_H */
