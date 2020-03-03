/*
 * driver/arm/device/smmuv2.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_H
#define DRIVER_ARM_DEVICE_SMMUV2_H

/* includes */

/* defines */

/* SMMU Global Register Space 0 */

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

/* SMMU Global Register Space 1 */

#define SMMU_CBAR(n)            ((n) * 4)
#define SMMU_CBFRSYNRA(n)       (0x0400 + ((n) * 4))
#define SMMU_CBA2R(n)           (0x0800 + ((n) * 4))

/* Stage 1 Translation Context Bank */

#define SMMU_S1_CB_SCTLR        0x00000
#define SMMU_S1_CB_ACTLR        0x00004
#define SMMU_S1_CB_RESUME       0x00008
#define SMMU_S1_CB_TCR2         0x00010
#define SMMU_S1_CB_TTBR0        0x00020
#define SMMU_S1_CB_TTBR1        0x00028
#define SMMU_S1_CB_TCR          0x00030
#define SMMU_S1_CB_CONTEXTIDR   0x00034
#define SMMU_S1_CB_PRRR         0x00038
#define SMMU_S1_CB_MAIR0        0x00038
#define SMMU_S1_CB_NMRR         0x0003C
#define SMMU_S1_CB_MAIR1        0x0003C
#define SMMU_S1_CB_PAR          0x00050
#define SMMU_S1_CB_FSR          0x00058
#define SMMU_S1_CB_FSRRESTORE   0x0005C
#define SMMU_S1_CB_FAR          0x00060
#define SMMU_S1_CB_FSYNR0       0x00068
#define SMMU_S1_CB_FSYNR1       0x0006C
#define SMMU_S1_CB_TLBIVA       0x00600
#define SMMU_S1_CB_TLBIVAA      0x00608
#define SMMU_S1_CB_TLBIASID     0x00610
#define SMMU_S1_CB_TLBIALL      0x00618
#define SMMU_S1_CB_TLBIVAL      0x00620
#define SMMU_S1_CB_TLBIVAAL     0x00628
#define SMMU_S1_CB_TLBSYNC      0x007F0
#define SMMU_S1_CB_TLBSTATUS    0x007F4
#define SMMU_S1_CB_ATS1PR       0x00800
#define SMMU_S1_CB_ATS1PW       0x00808
#define SMMU_S1_CB_ATS1UR       0x00810
#define SMMU_S1_CB_ATS1UW       0x00818
#define SMMU_S1_CB_ATSR         0x008F0
#define SMMU_S1_CB_PMEVCNTR(m)  (0x00E00 + ((m) * 4))
#define SMMU_S1_CB_PMEVTYPER(m) (0x00E80 + ((m) * 4))
#define SMMU_S1_CB_PMCFGR       0x00F00
#define SMMU_S1_CB_PMCR         0x00F04
#define SMMU_S1_CB_PMCEID0      0x00F20
#define SMMU_S1_CB_PMCEID1      0x00F24
#define SMMU_S1_CB_PMCNTENSET   0x00F40
#define SMMU_S1_CB_PMCNTENCLR   0x00F44
#define SMMU_S1_CB_PMINTENSET   0x00F48
#define SMMU_S1_CB_PMINTENCLR   0x00F4C
#define SMMU_S1_CB_PMOVSCLR     0x00F50
#define SMMU_S1_CB_PMOVSSET     0x00F58
#define SMMU_S1_CB_PMAUTHSTATUS 0x00FB8

/* Stage 2 Translation Context Bank */

#define SMMU_S2_CB_SCTLR        0x00000
#define SMMU_S2_CB_ACTLR        0x00004
#define SMMU_S2_CB_RESUME       0x00008
#define SMMU_S2_CB_TTBR0        0x00020
#define SMMU_S2_CB_TCR          0x00030
#define SMMU_S2_CB_FSR          0x00058
#define SMMU_S2_CB_FSRRESTORE   0x0005C
#define SMMU_S2_CB_FAR          0x00060
#define SMMU_S2_CB_FSYNR0       0x00068
#define SMMU_S2_CB_FSYNR1       0x0006C
#define SMMU_S2_CB_IPAFAR       0x00070
#define SMMU_S2_CB_TLBIIPAS2    0x00630
#define SMMU_S2_CB_TLBIIPAS2L   0x00638
#define SMMU_S2_CB_TLBSYNC      0x007F0
#define SMMU_S2_CB_TLBSTATUS    0x007F4
#define SMMU_S2_CB_PMEVCNTR(m)  (0x00E00 + ((m) * 4))
#define SMMU_S2_CB_PMEVTYPER(m) (0x00E80 + ((m) * 4))
#define SMMU_S2_CB_PMCFGR       0x00F00
#define SMMU_S2_CB_PMCR         0x00F04
#define SMMU_S2_CB_PMCEID0      0x00F20
#define SMMU_S2_CB_PMCEID1      0x00F24
#define SMMU_S2_CB_PMCNTENSET   0x00F40
#define SMMU_S2_CB_PMCNTENCLR   0x00F44
#define SMMU_S2_CB_PMINTENSET   0x00F48
#define SMMU_S2_CB_PMINTENCLR   0x00F4C
#define SMMU_S2_CB_PMOVSCLR     0x00F50
#define SMMU_S2_CB_PMOVSSET     0x00F58
#define SMMU_S2_CB_PMAUTHSTATUS 0x00FB8

#endif /* DRIVER_ARM_DEVICE_SMMUV2_H */

