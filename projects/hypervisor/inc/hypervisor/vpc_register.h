// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/vpc_register.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_VPC_REGISTER_H
#define HYPERVISOR_VPC_REGISTER_H

/* defines */

#define VPC_X0             0
#define VPC_X1             1
#define VPC_X2             2
#define VPC_X3             3
#define VPC_X4             4
#define VPC_X5             5
#define VPC_X6             6
#define VPC_X7             7
#define VPC_X8             8
#define VPC_X9             9
#define VPC_X10            10
#define VPC_X11            11
#define VPC_X12            12
#define VPC_X13            13
#define VPC_X14            14
#define VPC_X15            15
#define VPC_X16            16
#define VPC_IP0            VPC_X16
#define VPC_X17            17
#define VPC_IP1            VPC_X17
#define VPC_X18            18
#define VPC_X19            19
#define VPC_X20            20
#define VPC_X21            21
#define VPC_X22            22
#define VPC_X23            23
#define VPC_X24            24
#define VPC_X25            25
#define VPC_X26            26
#define VPC_X27            27
#define VPC_X28            28
#define VPC_X29            29
#define VPC_FP             VPC_X29
#define VPC_X30            30
#define VPC_LR             VPC_X30
#define VPC_SP_EL0         31
#define VPC_SP_EL1         32
#define VPC_PC             33
#define VPC_SPSR_EL2       34
#define VPC_ELR_EL1        35
#define VPC_SPSR_EL1       36
#define VPC_SPSR_ABT       37
#define VPC_SPSR_IRQ       38
#define VPC_SPSR_FIQ       39
#define VPC_SPSR_UND       40
#define VPC_CSSELR_EL1     41 // Cache Size Selection Register
#define VPC_ESR_EL1        42 // Exception Syndrome Register
#define VPC_FAR_EL1        43 // Fault Address Register
#define VPC_VBAR_EL1       44 // Vector Base Address Register
#define VPC_SCTLR_EL1      45 // System Control Register
#define VPC_TTBR0_EL1      46 // Translation Table Base Register 0
#define VPC_TTBR1_EL1      47 // Translation Table Base Register 1
#define VPC_TCR_EL1        48 // Translation Control Register
#define VPC_MAIR_EL1       49 // Memory Attribute Indirection Register
#define VPC_CONTEXTIDR_EL1 50 // Context ID Register
#define VPC_CPACR_EL1      51 // Architectural Feature Access Control Register
#define VPC_PAR_EL1        52 // Physical Address Register
#define VPC_TPIDR_EL0      53 // Thread Pointer/ID Register
#define VPC_TPIDR_EL1      54 // Thread Pointer/ID Register
#define VPC_TPIDRRO_EL0    55 // Thread Pointer/ID Register
#define VPC_CNTKCTL_EL1    56 // Counter-timer Kernel Control register
#define VPC_CNTP_CTL_EL0   57 // Counter-timer Physical Timer Control register
#define VPC_CNTP_CVAL_EL0 \
    58 // Counter-timer Physical Timer CompareValue register
#define VPC_CNTV_CTL_EL0 59 // Counter-timer Virtual Timer Control register
#define VPC_CNTV_CVAL_EL0 \
    60                    // Counter-timer Virtual Timer CompareValue register
#define VPC_VPIDR_EL2  61 // Virtualization Processor ID Register
#define VPC_VMPIDR_EL2 62 // Virtualization Multiprocessor ID Register
#define VPC_IFSR32_EL2 63 // Instruction Fault Status Register
#define VPC_ESR_EL2    64 // Exception Syndrome Register
#define VPC_FAR_EL2    65 // Fault Address Register
#define VPC_HPFAR_EL2  66 // Hypervisor IPA Fault Address Register
#define VPC_VTTBR_EL2 \
    67 // Virtualization Translation Table Base Address Register
#define VPC_VTCR_EL2    68 // Virtualization Translation Control Register
#define VPC_HCR_EL2     69 // Hypervisor Configuration Register
#define VPC_HSTR_EL2    70 // Hyp System Trap Register
#define VPC_HACR_EL2    71 // Hyp Auxiliary Configuration Register
#define VPC_DACR32_EL2  72 // Domain Access Control Register
#define VPC_CNTVOFF_EL2 73 // Counter-timer Virtual Offset register
#define VPC_FPCR        74 // Floating-point Control Register
#define VPC_FPSR        75 // Floating-point Status Register
#define VPC_FPEXC32_EL2 76 // Floating-point Exception Control Register
#define VPC_PAD         77
#define VPC_Q0          78
#define VPC_Q1          80
#define VPC_Q2          82
#define VPC_Q3          84
#define VPC_Q4          86
#define VPC_Q5          88
#define VPC_Q6          90
#define VPC_Q7          92
#define VPC_Q8          94
#define VPC_Q9          96
#define VPC_Q10         98
#define VPC_Q11         100
#define VPC_Q12         102
#define VPC_Q13         104
#define VPC_Q14         106
#define VPC_Q15         108
#define VPC_Q16         110
#define VPC_Q17         112
#define VPC_Q18         114
#define VPC_Q19         116
#define VPC_Q20         118
#define VPC_Q21         120
#define VPC_Q22         122
#define VPC_Q23         124
#define VPC_Q24         126
#define VPC_Q25         128
#define VPC_Q26         130
#define VPC_Q27         132
#define VPC_Q28         134
#define VPC_Q29         136
#define VPC_Q30         138
#define VPC_Q31         140
#define NR_VPC_REGS     142

#endif /* HYPERVISOR_VPC_REGISTER_H */
