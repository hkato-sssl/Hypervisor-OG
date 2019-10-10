/*
 * hypervisor/vm/register.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_VM_REGISTER_H
#define HYPERVISOR_VM_REGISTER_H

/* includes */

/* defines */

#define REGS_VECTOR         0
#define REGS_ESR            1
#define REGS_SPSR           2
#define REGS_ELR            3
#define REGS_X0             4
#define REGS_X1             (REGS_X0 + 1)
#define REGS_X2             (REGS_X0 + 2)
#define REGS_X3             (REGS_X0 + 3)
#define REGS_X4             (REGS_X0 + 4)
#define REGS_X5             (REGS_X0 + 5)
#define REGS_X6             (REGS_X0 + 6)
#define REGS_X7             (REGS_X0 + 7)
#define REGS_X8             (REGS_X0 + 8)
#define REGS_X9             (REGS_X0 + 9)
#define REGS_X10            (REGS_X0 + 10)
#define REGS_X11            (REGS_X0 + 11)
#define REGS_X12            (REGS_X0 + 12)
#define REGS_X13            (REGS_X0 + 13)
#define REGS_X14            (REGS_X0 + 14)
#define REGS_X15            (REGS_X0 + 15)
#define REGS_X16            (REGS_X0 + 16)
#define REGS_X17            (REGS_X0 + 17)
#define REGS_X18            (REGS_X0 + 18)
#define REGS_X19            (REGS_X0 + 19)
#define REGS_X20            (REGS_X0 + 20)
#define REGS_X21            (REGS_X0 + 21)
#define REGS_X22            (REGS_X0 + 22)
#define REGS_X23            (REGS_X0 + 23)
#define REGS_X24            (REGS_X0 + 24)
#define REGS_X25            (REGS_X0 + 25)
#define REGS_X26            (REGS_X0 + 26)
#define REGS_X27            (REGS_X0 + 27)
#define REGS_X28            (REGS_X0 + 28)
#define REGS_X29            (REGS_X0 + 29)
#define REGS_X30            (REGS_X0 + 30)
#define REGS_X31            (REGS_X0 + 31)
#define REGS_SP             REGS_X31

#define REGS_SYS_EL1_BASE   (REGS_X31 + 1)
#define REGS_SCTRL_EL1      REGS_SYS_BASE
#define REGS_ACTRL_EL1      (REGS_SYS_BASE + 1)

#define REGS_SYS_EL2_BASE   (REGS_ACTLRL_EL1 + 1)
#define	REGS_VPIDR_EL2		REGS_SYS_EL2_BASE
#define	REGS_VMPIDR_EL2		(REGS_SYS_EL2_BASE + 1)
#define	REGS_VTTBR_EL2		(REGS_SYS_EL2_BASE + 2)
#define	REGS_VTCR_EL2		(REGS_SYS_EL2_BASE + 3)
#define	REGS_HCR_EL2		(REGS_SYS_EL2_BASE + 4)
#define	REGS_HSTR_EL2		(REGS_SYS_EL2_BASE + 5)
#define	REGS_HACR_EL2		(REGS_SYS_EL2_BASE + 6)

#endif /* HYPERVISOR_VM_REGISTER_H */

