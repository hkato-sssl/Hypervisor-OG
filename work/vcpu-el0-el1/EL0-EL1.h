#define VCPU_CSSELR_EL1               VCPU_SYS_BASE            // Cache Size Selection Register
#define VCPU_ESR_EL1                  (VCPU_SYS_BASE + 1)      // Exception Syndrome Register
#define VCPU_FAR_EL1                  (VCPU_SYS_BASE + 2)      // Fault Address Register
#define VCPU_VBAR_EL1                 (VCPU_SYS_BASE + 3)      // Vector Base Address Register
#define VCPU_SCTLR_EL1                (VCPU_SYS_BASE + 4)      // System Control Register
#define VCPU_TTBR0_EL1                (VCPU_SYS_BASE + 5)      // Translation Table Base Register 0
#define VCPU_TTBR1_EL1                (VCPU_SYS_BASE + 6)      // Translation Table Base Register 1
#define VCPU_TCR_EL1                  (VCPU_SYS_BASE + 7)      // Translation Control Register
#define VCPU_MAIR_EL1                 (VCPU_SYS_BASE + 8)      // Memory Attribute Indirection Register
#define VCPU_CONTEXTIDR_EL1           (VCPU_SYS_BASE + 9)      // Context ID Register
#define VCPU_CPACR_EL1                (VCPU_SYS_BASE + 10)     // Architectural Feature Access Control Register
#define VCPU_PAR_EL1                  (VCPU_SYS_BASE + 11)     // Physical Address Register
#define VCPU_TPIDR_EL0                (VCPU_SYS_BASE + 12)     // Thread Pointer/ID Register
#define VCPU_TPIDR_EL1                (VCPU_SYS_BASE + 13)     // Thread Pointer/ID Register
#define VCPU_TPIDRRO_EL0              (VCPU_SYS_BASE + 14)     // Thread Pointer/ID Register
#define VCPU_CNTKCTL_EL1              (VCPU_SYS_BASE + 15)     // Counter-timer Kernel Control register
#define VCPU_CNTP_TVAL_EL0            (VCPU_SYS_BASE + 16)     // Counter-timer Physical Timer TimerValue register
#define VCPU_CNTP_CTL_EL0             (VCPU_SYS_BASE + 17)     // Counter-timer Physical Timer Control register
#define VCPU_CNTP_CVAL_EL0            (VCPU_SYS_BASE + 18)     // Counter-timer Physical Timer CompareValue register
#define VCPU_CNTV_TVAL_EL0            (VCPU_SYS_BASE + 19)     // Counter-timer Virtual Timer TimerValue register
#define VCPU_CNTV_CTL_EL0             (VCPU_SYS_BASE + 20)     // Counter-timer Virtual Timer Control register
#define VCPU_CNTV_CVAL_EL0            (VCPU_SYS_BASE + 21)     // Counter-timer Virtual Timer CompareValue register
#define VCPU_CNTPS_TVAL_EL1           (VCPU_SYS_BASE + 22)     // Counter-timer Physical Secure Timer TimerValue register
#define VCPU_CNTPS_CTL_EL1            (VCPU_SYS_BASE + 23)     // Counter-timer Physical Secure Timer Control register
#define VCPU_CNTPS_CVAL_EL1           (VCPU_SYS_BASE + 24)     // Counter-timer Physical Secure Timer CompareValue register
