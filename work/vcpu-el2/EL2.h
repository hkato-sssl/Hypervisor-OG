#define VCPU_VPIDR_EL2           VCPU_SYS_EL2_BASE        // Virtualization Processor ID Register
#define VCPU_VMPIDR_EL2          (VCPU_SYS_EL2_BASE + 1)  // Virtualization Multiprocessor ID Register
#define VCPU_IFSR32_EL2          (VCPU_SYS_EL2_BASE + 2)  // Instruction Fault Status Register
#define VCPU_HPFAR_EL2           (VCPU_SYS_EL2_BASE + 3)  // Hypervisor IPA Fault Address Register
#define VCPU_VTTBR_EL2           (VCPU_SYS_EL2_BASE + 4)  // Virtualization Translation Table Base Address Register
#define VCPU_VTCR_EL2            (VCPU_SYS_EL2_BASE + 5)  // Virtualization Translation Control Register
#define VCPU_HCR_EL2             (VCPU_SYS_EL2_BASE + 6)  // Hypervisor Configuration Register
#define VCPU_HSTR_EL2            (VCPU_SYS_EL2_BASE + 7)  // Hyp System Trap Register
#define VCPU_HACR_EL2            (VCPU_SYS_EL2_BASE + 8)  // Hyp Auxiliary Configuration Register
#define VCPU_DACR32_EL2          (VCPU_SYS_EL2_BASE + 9)  // Domain Access Control Register
#define VCPU_CNTVOFF_EL2         (VCPU_SYS_EL2_BASE + 10) // Counter-timer Virtual Offset register
#define VCPU_FPU_BASE            (VCPU_SYS_EL2_BASE + 11)
