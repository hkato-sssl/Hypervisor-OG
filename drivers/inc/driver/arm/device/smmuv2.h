/*
 * driver/arm/device/smmuv2.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_H
#define DRIVER_ARM_DEVICE_SMMUV2_H

/* includes */

/* defines */

#define SMMU_sCR0               0x000
#define SMMU_SCR1               0x004
#define SMMU_sCR2               0x008
#define SMMU_sACR               0x010
#define SMMU_IDR0               0x020
#define SMMU_IDR1               0x024
#define SMMU_IDR2               0x028
#define SMMU_IDR3               0x02C
#define SMMU_IDR4               0x030
#define SMMU_IDR5               0x034
#define SMMU_IDR6               0x038
#define SMMU_IDR7               0x03C
#define SMMU_sGFAR              0x040
#define SMMU_sGFSR              0x048
#define SMMU_sGFSRRESTORE       0x04C
#define SMMU_sGFSYNR0           0x050
#define SMMU_sGFSYNR1           0x054
#define SMMU_sGFSYNR2           0x058
#define SMMU_STLBIALL           0x060
#define SMMU_TLBIVMID           0x064
#define SMMU_TLBIALLNSNH        0x068
#define SMMU_TLBIALLH           0x06C
#define SMMU_sTLBGSYNC          0x070
#define SMMU_sTLBGSTATUS        0x074
#define SMMU_TLBIVAH            0x078
#define SMMU_STLBIVALM          0x0A0
#define SMMU_STLBIVAM           0x0A8
#define SMMU_TLBIVALH64         0x0B0
#define SMMU_TLBIVMIDS1         0x0B8
#define SMMU_STLBIALLM          0x0BC
#define SMMU_TLBIVAH64          0x0C0
#define SMMU_sGATS1UR           0x100
#define SMMU_sGATS1UW           0x108
#define SMMU_sGATS1PR           0x110
#define SMMU_sGATS1PW           0x118
#define SMMU_sGATS12UR          0x120
#define SMMU_sGATS12UW          0x128
#define SMMU_sGATS12PR          0x130
#define SMMU_sGATS12PW          0x138
#define SMMU_sGPAR              0x180
#define SMMU_sGATSR             0x188
#define SMMU_NSCR0              0x400
#define SMMU_NSCR2              0x408
#define SMMU_NSACR              0x410
#define SMMU_NSGFAR             0x440
#define SMMU_NSGFSR             0x448
#define SMMU_NSGFSRRESTORE      0x44C
#define SMMU_NSGFSYNR0          0x450
#define SMMU_NSGFSYNR1          0x454
#define SMMU_NSGFSYNR2          0x458
#define SMMU_NSTLBGSYNC         0x470
#define SMMU_NSTLBGSTATUS       0x474
#define SMMU_NSGATS1UR          0x500
#define SMMU_NSGATS1UW          0x508
#define SMMU_NSGATS1PR          0x510
#define SMMU_NSGATS1PW          0x518
#define SMMU_NSGATS12UR         0x520
#define SMMU_NSGATS12UW         0x528
#define SMMU_NSGATS12PR         0x530
#define SMMU_NSGATS12PW         0x538
#define SMMU_NSGPAR             0x580
#define SMMU_NSGATSR            0x588
#define SMMU_SMR(n)             (0x800 + (n) * 4)
#define SMMU_S2CR(n)            (0xC00 * (n) * 4)

#endif /* DRIVER_ARM_DEVICE_SMMUV2_H */

