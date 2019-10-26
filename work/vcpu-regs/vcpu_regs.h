#define VCPU_X0                 0
#define VCPU_X1                 1
#define VCPU_X2                 2
#define VCPU_X3                 3
#define VCPU_X4                 4
#define VCPU_X5                 5
#define VCPU_X6                 6
#define VCPU_X7                 7
#define VCPU_X8                 8
#define VCPU_X9                 9
#define VCPU_X10                10
#define VCPU_X11                11
#define VCPU_X12                12
#define VCPU_X13                13
#define VCPU_X14                14
#define VCPU_X15                15
#define VCPU_X16                16
#define VCPU_X17                17
#define VCPU_X18                18
#define VCPU_X19                19
#define VCPU_X20                20
#define VCPU_X21                21
#define VCPU_X22                22
#define VCPU_X23                23
#define VCPU_X24                24
#define VCPU_X25                25
#define VCPU_X26                26
#define VCPU_X27                27
#define VCPU_X28                28
#define VCPU_X29                29
#define VCPU_X30                30
#define VCPU_SP_EL0             31
#define VCPU_SP_EL1             32
#define VCPU_PC                 33
#define VCPU_ELR_EL1            34
#define VCPU_SPSR_EL1           35
#define VCPU_SPSR_ABT           36
#define VCPU_SPSR_IRQ           37
#define VCPU_SPSR_FIQ           38
#define VCPU_SPSR_UND           39
#define VCPU_CSSELR_EL1         40      // Cache Size Selection Register
#define VCPU_ESR_EL1            41      // Exception Syndrome Register
#define VCPU_FAR_EL1            42      // Fault Address Register
#define VCPU_VBAR_EL1           43      // Vector Base Address Register
#define VCPU_SCTLR_EL1          44      // System Control Register
#define VCPU_TTBR0_EL1          45      // Translation Table Base Register 0
#define VCPU_TTBR1_EL1          46      // Translation Table Base Register 1
#define VCPU_TCR_EL1            47      // Translation Control Register
#define VCPU_MAIR_EL1           48      // Memory Attribute Indirection Register
#define VCPU_CONTEXTIDR_EL1     49      // Context ID Register
#define VCPU_CPACR_EL1          50      // Architectural Feature Access Control Register
#define VCPU_PAR_EL1            51      // Physical Address Register
#define VCPU_TPIDR_EL0          52      // Thread Pointer/ID Register
#define VCPU_TPIDR_EL1          53      // Thread Pointer/ID Register
#define VCPU_TPIDRRO_EL0        54      // Thread Pointer/ID Register
#define VCPU_CNTKCTL_EL1        55      // Counter-timer Kernel Control register
#define VCPU_CNTP_TVAL_EL0      56      // Counter-timer Physical Timer TimerValue register
#define VCPU_CNTP_CTL_EL0       57      // Counter-timer Physical Timer Control register
#define VCPU_CNTP_CVAL_EL0      58      // Counter-timer Physical Timer CompareValue register
#define VCPU_CNTV_TVAL_EL0      59      // Counter-timer Virtual Timer TimerValue register
#define VCPU_CNTV_CTL_EL0       60      // Counter-timer Virtual Timer Control register
#define VCPU_CNTV_CVAL_EL0      61      // Counter-timer Virtual Timer CompareValue register
#define VCPU_VPIDR_EL2          62      // Virtualization Processor ID Register
#define VCPU_VMPIDR_EL2         63      // Virtualization Multiprocessor ID Register
#define VCPU_IFSR32_EL2         64      // Instruction Fault Status Register
#define VCPU_HPFAR_EL2          65      // Hypervisor IPA Fault Address Register
#define VCPU_VTTBR_EL2          66      // Virtualization Translation Table Base Address Register
#define VCPU_VTCR_EL2           67      // Virtualization Translation Control Register
#define VCPU_HCR_EL2            68      // Hypervisor Configuration Register
#define VCPU_HSTR_EL2           69      // Hyp System Trap Register
#define VCPU_HACR_EL2           70      // Hyp Auxiliary Configuration Register
#define VCPU_DACR32_EL2         71      // Domain Access Control Register
#define VCPU_CNTVOFF_EL2        72      // Counter-timer Virtual Offset register
#define VCPU_FPCR               73      // Floating-point Control Register 
#define VCPU_FPSR               74      // Floating-point Status Register 
#define VCPU_FPEXC32_EL2        75      // Floating-point Exception Control Register
#define VCPU_Q0                 76
#define VCPU_Q1                 78
#define VCPU_Q2                 80
#define VCPU_Q3                 82
#define VCPU_Q4                 84
#define VCPU_Q5                 86
#define VCPU_Q6                 88
#define VCPU_Q7                 90
#define VCPU_Q8                 92
#define VCPU_Q9                 94
#define VCPU_Q10                96
#define VCPU_Q11                98
#define VCPU_Q12                100
#define VCPU_Q13                102
#define VCPU_Q14                104
#define VCPU_Q15                106
#define VCPU_Q16                108
#define VCPU_Q17                110
#define VCPU_Q18                112
#define VCPU_Q19                114
#define VCPU_Q20                116
#define VCPU_Q21                118
#define VCPU_Q22                120
#define VCPU_Q23                122
#define VCPU_Q24                124
#define VCPU_Q25                126
#define VCPU_Q26                128
#define VCPU_Q27                130
#define VCPU_Q28                132
#define VCPU_Q29                134
#define VCPU_Q30                136
#define VCPU_Q31                138
#define NR_VCPU_REGS            140
