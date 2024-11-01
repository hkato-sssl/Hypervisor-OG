// SPDX-License-Identifier: Apache-2.0
/*
 * driver/aarch64/system_register.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_H

/* includes */

#include "lib/bit.h"

/* defines */

#define CURRENT_EL0    0
#define CURRENT_EL1    (1 << 2)
#define CURRENT_EL2    (2 << 2)
#define CURRENT_EL3    (3 << 2)

/* PSTATE */

#define PSTATE_N       BIT(31)
#define PSTATE_Z       BIT(30)
#define PSTATE_C       BIT(29)
#define PSTATE_V       BIT(28)
#define PSTATE_DIT     BIT(24) /* ARMv8.4 */
#define PSTATE_UAO     BIT(23) /* ARMv8.2 */
#define PSTATE_PAN     BIT(22) /* ARMv8.1 */
#define PSTATE_D       BIT(9)
#define PSTATE_A       BIT(8)
#define PSTATE_I       BIT(7)
#define PSTATE_F       BIT(6)
#define PSTATE_SP      BIT(0)

/* SPSR */

#define SPSR_M_EL3h    0x0d
#define SPSR_M_EL3t    0x0c
#define SPSR_M_EL2h    0x09
#define SPSR_M_EL2t    0x08
#define SPSR_M_EL1h    0x05
#define SPSR_M_EL1t    0x04
#define SPSR_M_EL0t    0x00

/* DAIFset/DAIFclr */

#define DAIF_D         BIT(3)
#define DAIF_A         BIT(2)
#define DAIF_I         BIT(1)
#define DAIF_F         BIT(0)
#define DAIF_ALL       BITS(3, 0)

/* SCTLR */
#define SCTLR_EnIA     BIT(31)
#define SCTLR_EnIB     BIT(30)
#define SCTLR_LSMAOE   BIT(29)
#define SCTLR_nTLSMD   BIT(28)
#define SCTLR_EnDA     BIT(27)
#define SCTLR_UCI      BIT(26)
#define SCTLR_EE       BIT(25)
#define SCTLR_E0E      BIT(24)
#define SCTLR_SPAN     BIT(23)
#define SCTLR_IESB     BIT(21)
#define SCTLR_WXN      BIT(19)
#define SCTLR_nTWE     BIT(18)
#define SCTLR_nTWI     BIT(16)
#define SCTLR_UCT      BIT(15)
#define SCTLR_DZE      BIT(14)
#define SCTLR_EnDB     BIT(13)
#define SCTLR_I        BIT(12)
#define SCTLR_UMA      BIT(9)
#define SCTLR_SED      BIT(8)
#define SCTLR_ITD      BIT(7)
#define SCTLR_nAA      BIT(6)
#define SCTLR_CP15BEN  BIT(5)
#define SCTLR_SA0      BIT(4)
#define SCTLR_SA       BIT(3)
#define SCTLR_C        BIT(2)
#define SCTLR_A        BIT(1)
#define SCTLR_M        BIT(0)
#define SCTLR_EL1_RES1 (BITS(29, 28) | BITS(23, 22) | BIT(20) | BIT(11))

/* ACTLR */
#define ACTLR_L2ACTLR  BIT(6)
#define ACTLR_L2ECTLR  BIT(5)
#define ACTLR_L2CTLR   BIT(4)
#define ACTLR_CPUECTLR BIT(1)
#define ACTLR_CPUACTLR BIT(0)

/* CPTR */
#define CPTR_TCPAC     BIT(31)
#define CPTR_TTA       BIT(20)
#define CPTR_TFP       BIT(10)
#define CPTR_RES1_EL2  BITS(13, 12)

/* HCR_EL2 */
#define HCR_MIOCNCE    BIT(38)
#define HCR_ID         BIT(33)
#define HCR_CD         BIT(32)
#define HCR_RW         BIT(31)
#define HCR_TRVM       BIT(30)
#define HCR_HCD        BIT(29)
#define HCR_TDZ        BIT(28)
#define HCR_TGE        BIT(27)
#define HCR_TVM        BIT(26)
#define HCR_TTLB       BIT(25)
#define HCR_TPU        BIT(24)
#define HCR_TPC        BIT(23)
#define HCR_TSW        BIT(22)
#define HCR_TACR       BIT(21)
#define HCR_TIDCP      BIT(20)
#define HCR_TSC        BIT(19)
#define HCR_TID3       BIT(18)
#define HCR_TID2       BIT(17)
#define HCR_TID1       BIT(16)
#define HCR_TID0       BIT(15)
#define HCR_TWE        BIT(14)
#define HCR_TWI        BIT(13)
#define HCR_DC         BIT(12)
#define HCR_BSU_MASK   BITS(11, 10)
#define HCR_BSU_MSB    11
#define HCR_BSU_LSB    10
#define HCR_FB         BIT(9)
#define HCR_VSE        BIT(8)
#define HCR_VI         BIT(7)
#define HCR_VF         BIT(6)
#define HCR_AMO        BIT(5)
#define HCR_IMO        BIT(4)
#define HCR_FMO        BIT(3)
#define HCR_PTW        BIT(2)
#define HCR_SWIO       BIT(1)
#define HCR_VM         BIT(0)

#ifndef ASSEMBLY

/* includes */

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* types */

/* variables */

/* functions */

/*
 * AArch64 identification registers
 */

/* Saved Program Status Register */

static inline void aarch64_write_spsr_el1(uint32_t d)
{
    __asm volatile("msr SPSR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_spsr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, SPSR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Exception Link Register */

static inline void aarch64_write_elr_el1(uint64_t d)
{
    __asm volatile("msr ELR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_elr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ELR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Main ID Register */

static inline uint32_t aarch64_read_midr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, MIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Multiprocessor Affinity Register */

static inline uint64_t aarch64_read_mpidr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, MPIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Revision ID Register */

static inline uint32_t aarch64_read_revidr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, REVIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Processor Feature Register 0 */

static inline uint32_t aarch64_read_id_pfr0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_PFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Processor Feature Register 1 */

static inline uint32_t aarch64_read_id_pfr1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_PFR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Debug Feature Register 0 */

static inline uint32_t aarch64_read_id_dfr0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_DFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Auxiliary Feature Register 0 */

static inline uint32_t aarch64_read_id_afr0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_AFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Memory Model Feature Register 0 */

static inline uint32_t aarch64_read_id_mmfr0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_MMFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Memory Model Feature Register 1 */

static inline uint32_t aarch64_read_id_mmfr1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_MMFR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Memory Model Feature Register 2 */

static inline uint32_t aarch64_read_id_mmfr2_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_MMFR2_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Memory Model Feature Register 3 */

static inline uint32_t aarch64_read_id_mmfr3_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_MMFR3_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Instruction Set Attribute Register 0 */

static inline uint32_t aarch64_read_id_isar0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_ISAR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Instruction Set Attribute Register 1 */

static inline uint32_t aarch64_read_id_isar1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_ISAR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Instruction Set Attribute Register 2 */

static inline uint32_t aarch64_read_id_isar2_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_ISAR2_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Instruction Set Attribute Register 3 */

static inline uint32_t aarch64_read_id_isar3_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_ISAR3_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Instruction Set Attribute Register 4 */

static inline uint32_t aarch64_read_id_isar4_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_ISAR4_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch32 Instruction Set Attribute Register 5 */

static inline uint32_t aarch64_read_id_isar5_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ID_ISAR5_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Processor Feature Register 0 */

static inline uint64_t aarch64_read_id_aa64pfr0_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64PFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Processor Feature Register 1 */

static inline uint64_t aarch64_read_id_aa64pfr1_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64PFR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Debug Feature Register 0 */

static inline uint64_t aarch64_read_id_aa64dfr0_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64DFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Debug Feature Register 1 */

static inline uint64_t aarch64_read_id_aa64dfr1_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64DFR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Auxiliary Feature Register 0 */

static inline uint64_t aarch64_read_id_aa64afr0_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64AFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Auxiliary Feature Register 1 */

static inline uint64_t aarch64_read_id_aa64afr1_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64AFR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Instruction Set Attribute Register 0 */

static inline uint64_t aarch64_read_id_aa64isar0_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64ISAR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Instruction Set Attribute Register 1 */

static inline uint64_t aarch64_read_id_aa64isar1_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64ISAR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Memory Model Feature Register 0 */

static inline uint64_t aarch64_read_id_aa64mmfr0_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64MMFR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* AArch64 Memory Model Feature Register 1 */

static inline uint64_t aarch64_read_id_aa64mmfr1_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ID_AA64MMFR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* Cache Size ID Register */

static inline uint32_t aarch64_read_ccsidr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CCSIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Cache Level ID Register */

static inline uint64_t aarch64_read_clidr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CLIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary ID Register */

static inline uint32_t aarch64_read_aidr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, AIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Cache Size Selection Register */

static inline void aarch64_write_csselr_el1(uint32_t d)
{
    __asm volatile("msr CSSELR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_csselr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CSSELR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Cache Type Register */

static inline uint32_t aarch64_read_ctr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CTR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Data Cache Zero ID Register */

static inline uint32_t aarch64_read_dczid_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, DCZID_EL0" : "=r"(d)::"memory");
    return d;
}

/* Virtualization Processor ID Register */

static inline void aarch64_write_vpidr_el2(uint32_t d)
{
    __asm volatile("msr VPIDR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_vpidr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, VPIDR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Virtualization Multiprocessor ID Register */

static inline uint64_t aarch64_read_vmpidr_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, VMPIDR_EL2" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 exception handling registers
 */

/* Auxiliary Fault Status Register 0 */

static inline void aarch64_write_afsr0_el1(uint32_t d)
{
    __asm volatile("msr AFSR0_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_afsr0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, AFSR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Fault Status Register 1 */

static inline void aarch64_write_afsr1_el1(uint32_t d)
{
    __asm volatile("msr AFSR1_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_afsr1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, AFSR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* Exception Syndrome Register */

static inline void aarch64_write_esr_el1(uint32_t d)
{
    __asm volatile("msr ESR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_esr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ESR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Instruction Fault Status Register */

static inline void aarch64_write_ifsr32_el2(uint32_t d)
{
    __asm volatile("msr IFSR32_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_ifsr32_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, IFSR32_EL2" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Fault Status Register 0 */

static inline void aarch64_write_afsr0_el2(uint32_t d)
{
    __asm volatile("msr AFSR0_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_afsr0_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, AFSR0_EL2" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Fault Status Register 1 */

static inline void aarch64_write_afsr1_el2(uint32_t d)
{
    __asm volatile("msr AFSR1_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_afsr1_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, AFSR1_EL2" : "=r"(d)::"memory");
    return d;
}

/* Exception Syndrome Register */

static inline void aarch64_write_esr_el2(uint32_t d)
{
    __asm volatile("msr ESR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_esr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ESR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Fault Status Register 0 */

static inline void aarch64_write_afsr0_el3(uint32_t d)
{
    __asm volatile("msr AFSR0_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_afsr0_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, AFSR0_EL3" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Fault Status Register 1 */

static inline void aarch64_write_afsr1_el3(uint32_t d)
{
    __asm volatile("msr AFSR1_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_afsr1_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, AFSR1_EL3" : "=r"(d)::"memory");
    return d;
}

/* Exception Syndrome Register */

static inline void aarch64_write_esr_el3(uint32_t d)
{
    __asm volatile("msr ESR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_esr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ESR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Fault Address Register */

static inline void aarch64_write_far_el1(uint64_t d)
{
    __asm volatile("msr FAR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_far_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, FAR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Fault Address Register */

static inline void aarch64_write_far_el2(uint64_t d)
{
    __asm volatile("msr FAR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_far_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, FAR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Hypervisor IPA Fault Address Register */

static inline void aarch64_write_hpfar_el2(uint64_t d)
{
    __asm volatile("msr HPFAR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_hpfar_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, HPFAR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Fault Address Register */

static inline void aarch64_write_far_el3(uint64_t d)
{
    __asm volatile("msr FAR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_far_el3(void)
{
    uint64_t d;
    __asm volatile("mrs %0, FAR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Vector Base Address Register */

static inline void aarch64_write_vbar_el1(uint64_t d)
{
    __asm volatile("msr VBAR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_vbar_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, VBAR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Status Register */

static inline uint32_t aarch64_read_isr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ISR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Vector Base Address Register */

static inline void aarch64_write_vbar_el2(uint64_t d)
{
    __asm volatile("msr VBAR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_vbar_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, VBAR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Vector Base Address Register */

static inline void aarch64_write_vbar_el3(uint64_t d)
{
    __asm volatile("msr VBAR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_vbar_el3(void)
{
    uint64_t d;
    __asm volatile("mrs %0, VBAR_EL3" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 virtual memory control registers
 */

/* System Control Register */

static inline void aarch64_write_sctlr_el1(uint32_t d)
{
    __asm volatile("msr SCTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_sctlr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, SCTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* System Control Register */

static inline void aarch64_write_sctlr_el2(uint32_t d)
{
    __asm volatile("msr SCTLR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_sctlr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, SCTLR_EL2" : "=r"(d)::"memory");
    return d;
}

/* System Control Register */

static inline void aarch64_write_sctlr_el3(uint32_t d)
{
    __asm volatile("msr SCTLR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_sctlr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, SCTLR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Translation Table Base Register 0 */

static inline void aarch64_write_ttbr0_el1(uint64_t d)
{
    __asm volatile("msr TTBR0_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ttbr0_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TTBR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Translation Table Base Register 1 */

static inline void aarch64_write_ttbr1_el1(uint64_t d)
{
    __asm volatile("msr TTBR1_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ttbr1_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TTBR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* Translation Control Register */

static inline void aarch64_write_tcr_el1(uint64_t d)
{
    __asm volatile("msr TCR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_tcr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TCR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Translation Table Base Address Register 0 */

static inline void aarch64_write_ttbr0_el2(uint64_t d)
{
    __asm volatile("msr TTBR0_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ttbr0_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TTBR0_EL2" : "=r"(d)::"memory");
    return d;
}

/* Translation Control Register */

static inline void aarch64_write_tcr_el2(uint32_t d)
{
    __asm volatile("msr TCR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_tcr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, TCR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Virtualization Translation Table Base Address Register */

static inline void aarch64_write_vttbr_el2(uint64_t d)
{
    __asm volatile("msr VTTBR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_vttbr_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, VTTBR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Virtualization Translation Control Register */

static inline void aarch64_write_vtcr_el2(uint32_t d)
{
    __asm volatile("msr VTCR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_vtcr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, VTCR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Translation Table Base Register 0 */

static inline void aarch64_write_ttbr0_el3(uint64_t d)
{
    __asm volatile("msr TTBR0_EL3, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ttbr0_el3(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TTBR0_EL3" : "=r"(d)::"memory");
    return d;
}

/* Translation Control Register */

static inline void aarch64_write_tcr_el3(uint32_t d)
{
    __asm volatile("msr TCR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_tcr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, TCR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Memory Attribute Indirection Register */

static inline void aarch64_write_mair_el1(uint64_t d)
{
    __asm volatile("msr MAIR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_mair_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, MAIR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Memory Attribute Indirection Register */

static inline void aarch64_write_amair_el1(uint64_t d)
{
    __asm volatile("msr AMAIR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_amair_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, AMAIR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Memory Attribute Indirection Register */

static inline void aarch64_write_mair_el2(uint64_t d)
{
    __asm volatile("msr MAIR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_mair_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, MAIR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Memory Attribute Indirection Register */

static inline void aarch64_write_amair_el2(uint64_t d)
{
    __asm volatile("msr AMAIR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_amair_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, AMAIR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Memory Attribute Indirection Register */

static inline void aarch64_write_mair_el3(uint64_t d)
{
    __asm volatile("msr MAIR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_mair_el3(void)
{
    uint64_t d;
    __asm volatile("mrs %0, MAIR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Memory Attribute Indirection Register */

static inline void aarch64_write_amair_el3(uint64_t d)
{
    __asm volatile("msr AMAIR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_amair_el3(void)
{
    uint64_t d;
    __asm volatile("mrs %0, AMAIR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Context ID Register */

static inline void aarch64_write_contextidr_el1(uint32_t d)
{
    __asm volatile("msr CONTEXTIDR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_contextidr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CONTEXTIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 other system control registers
 */

/* Auxiliary Control Register */

static inline void aarch64_write_actlr_el1(uint32_t d)
{
    __asm volatile("msr ACTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_actlr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ACTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Architectural Feature Access Control Register */

static inline void aarch64_write_cpacr_el1(uint32_t d)
{
    __asm volatile("msr CPACR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cpacr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CPACR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Control Register */

static inline void aarch64_write_actlr_el2(uint32_t d)
{
    __asm volatile("msr ACTLR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_actlr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ACTLR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Auxiliary Control Register */

static inline void aarch64_write_actlr_el3(uint32_t d)
{
    __asm volatile("msr ACTLR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_actlr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ACTLR_EL3" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 address translation operations
 */

/* Physical Address Register */

static inline void aarch64_write_par_el1(uint64_t d)
{
    __asm volatile("msr PAR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_par_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, PAR_EL1" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 miscellaneous operations
 */

/* Thread Pointer/ID Register */

static inline void aarch64_write_tpidr_el0(uint64_t d)
{
    __asm volatile("msr TPIDR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_tpidr_el0(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TPIDR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Thread Pointer/ID Register */

static inline void aarch64_write_tpidr_el1(uint64_t d)
{
    __asm volatile("msr TPIDR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_tpidr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TPIDR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Thread Pointer/ID Register */

static inline void aarch64_write_tpidrro_el0(uint64_t d)
{
    __asm volatile("msr TPIDRRO_EL0, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_tpidrro_el0(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TPIDRRO_EL0" : "=r"(d)::"memory");
    return d;
}

/* Thread Pointer/ID Register */

static inline void aarch64_write_tpidr_el2(uint64_t d)
{
    __asm volatile("msr TPIDR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_tpidr_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TPIDR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Thread Pointer/ID Register */

static inline void aarch64_write_tpidr_el3(uint64_t d)
{
    __asm volatile("msr TPIDR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_tpidr_el3(void)
{
    uint64_t d;
    __asm volatile("mrs %0, TPIDR_EL3" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 performance monitor registers
 */

/* Performance Monitors Control Register */

static inline void aarch64_write_pmcr_el0(uint32_t d)
{
    __asm volatile("msr PMCR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmcr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMCR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Count Enable Set Register */

static inline void aarch64_write_pmcntenset_el0(uint32_t d)
{
    __asm volatile("msr PMCNTENSET_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmcntenset_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMCNTENSET_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Count Enable Clear Register */

static inline void aarch64_write_pmcntenclr_el0(uint32_t d)
{
    __asm volatile("msr PMCNTENCLR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmcntenclr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMCNTENCLR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Overflow Flag Status Clear Register */

static inline void aarch64_write_pmovsclr_el0(uint32_t d)
{
    __asm volatile("msr PMOVSCLR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmovsclr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMOVSCLR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Software Increment Register */

static inline void aarch64_write_pmswinc_el0(uint32_t d)
{
    __asm volatile("msr PMSWINC_EL0, %0" ::"r"(d) : "memory");
}

/* Performance Monitors Event Counter Selection Register */

static inline void aarch64_write_pmselr_el0(uint32_t d)
{
    __asm volatile("msr PMSELR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmselr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMSELR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Common Event Identification Register 0 */

static inline uint32_t aarch64_read_pmceid0_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMCEID0_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Common Event Identification Register 1 */

static inline uint32_t aarch64_read_pmceid1_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMCEID1_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Cycle Countera */

static inline void aarch64_write_pmccntr_el0(uint64_t d)
{
    __asm volatile("msr PMCCNTR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_pmccntr_el0(void)
{
    uint64_t d;
    __asm volatile("mrs %0, PMCCNTR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Selected Event Type and Filter Register */

static inline void aarch64_write_pmxevtyper_el0(uint32_t d)
{
    __asm volatile("msr PMXEVTYPER_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmxevtyper_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMXEVTYPER_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Selected Event Counter Register */

static inline void aarch64_write_pmxevcntr_el0(uint32_t d)
{
    __asm volatile("msr PMXEVCNTR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmxevcntr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMXEVCNTR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors User Enable Register */

static inline void aarch64_write_pmuserenr_el0(uint32_t d)
{
    __asm volatile("msr PMUSERENR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmuserenr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMUSERENR_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Interrupt Enable Set Register */

static inline void aarch64_write_pmintenset_el1(uint32_t d)
{
    __asm volatile("msr PMINTENSET_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmintenset_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMINTENSET_EL1" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Interrupt Enable Clear Register */

static inline void aarch64_write_pmintenclr_el1(uint32_t d)
{
    __asm volatile("msr PMINTENCLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmintenclr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMINTENCLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Overflow Flag Status Set Register */

static inline void aarch64_write_pmovsset_el0(uint32_t d)
{
    __asm volatile("msr PMOVSSET_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmovsset_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMOVSSET_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitor Event Count Registers */

static inline void aarch64_write_pmevcntr0_el0(uint32_t d)
{
    __asm volatile("msr PMEVCNTR0_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevcntr0_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVCNTR0_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevcntr1_el0(uint32_t d)
{
    __asm volatile("msr PMEVCNTR1_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevcntr1_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVCNTR1_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevcntr2_el0(uint32_t d)
{
    __asm volatile("msr PMEVCNTR2_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevcntr2_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVCNTR2_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevcntr3_el0(uint32_t d)
{
    __asm volatile("msr PMEVCNTR3_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevcntr3_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVCNTR3_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevcntr4_el0(uint32_t d)
{
    __asm volatile("msr PMEVCNTR4_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevcntr4_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVCNTR4_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevcntr5_el0(uint32_t d)
{
    __asm volatile("msr PMEVCNTR5_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevcntr5_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVCNTR5_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitor Event Type Registers */

static inline void aarch64_write_pmevtyper0_el0(uint32_t d)
{
    __asm volatile("msr PMEVTYPER0_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevtyper0_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVTYPER0_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevtyper1_el0(uint32_t d)
{
    __asm volatile("msr PMEVTYPER1_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevtyper1_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVTYPER1_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevtyper2_el0(uint32_t d)
{
    __asm volatile("msr PMEVTYPER2_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevtyper2_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVTYPER2_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevtyper3_el0(uint32_t d)
{
    __asm volatile("msr PMEVTYPER3_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevtyper3_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVTYPER3_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevtyper4_el0(uint32_t d)
{
    __asm volatile("msr PMEVTYPER4_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevtyper4_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVTYPER4_EL0" : "=r"(d)::"memory");
    return d;
}

static inline void aarch64_write_pmevtyper5_el0(uint32_t d)
{
    __asm volatile("msr PMEVTYPER5_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmevtyper5_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMEVTYPER5_EL0" : "=r"(d)::"memory");
    return d;
}

/* Performance Monitors Cycle Count Filter Register */

static inline void aarch64_write_pmccfiltr_el0(uint32_t d)
{
    __asm volatile("msr PMCCFILTR_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_pmccfiltr_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, PMCCFILTR_EL0" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 reset registers
 */

/* Reset Vector Base Address Register */

static inline uint64_t aarch64_read_rvbar_el3(void)
{
    uint64_t d;
    __asm volatile("mrs %0, RVBAR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Reset Management Register */

static inline void aarch64_write_rmr_el3(uint32_t d)
{
    __asm volatile("msr RMR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_rmr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, RMR_EL3" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 secure registers
 */

/* Secure Configuration Register */

static inline void aarch64_write_scr_el3(uint32_t d)
{
    __asm volatile("msr SCR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_scr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, SCR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Secure Debug Enable Register */

static inline void aarch64_write_sder32_el3(uint32_t d)
{
    __asm volatile("msr SDER32_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_sder32_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, SDER32_EL3" : "=r"(d)::"memory");
    return d;
}

/* Architectural Feature Trap Register */

static inline void aarch64_write_cptr_el3(uint32_t d)
{
    __asm volatile("msr CPTR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cptr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CPTR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Monitor Debug Configuration Register */

static inline void aarch64_write_mdcr_el3(uint32_t d)
{
    __asm volatile("msr MDCR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_mdcr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, MDCR_EL3" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 virtualization registers
 */

/* Hypervisor Configuration Register */

static inline void aarch64_write_hcr_el2(uint64_t d)
{
    __asm volatile("msr HCR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_hcr_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, HCR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Hyp Debug Control Register */

static inline void aarch64_write_mdcr_el2(uint32_t d)
{
    __asm volatile("msr MDCR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_mdcr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, MDCR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Architectural Feature Trap Register */

static inline void aarch64_write_cptr_el2(uint32_t d)
{
    __asm volatile("msr CPTR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cptr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CPTR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Hyp System Trap Register */

static inline void aarch64_write_hstr_el2(uint32_t d)
{
    __asm volatile("msr HSTR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_hstr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, HSTR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Hyp Auxiliary Configuration Register */

static inline void aarch64_write_hacr_el2(uint32_t d)
{
    __asm volatile("msr HACR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_hacr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, HACR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Domain Access Control Register */

static inline void aarch64_write_dacr32_el2(uint32_t d)
{
    __asm volatile("msr DACR32_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_dacr32_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, DACR32_EL2" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 GIC system registers
 */

/* Active Priorities 0 Register 0 */

static inline void aarch64_write_icc_ap0r0_el1(uint32_t d)
{
    __asm volatile("msr ICC_AP0R0_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_ap0r0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_AP0R0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Active Priorities 1 Register 0 */

static inline void aarch64_write_icc_ap1r0_el1(uint32_t d)
{
    __asm volatile("msr ICC_AP1R0_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_ap1r0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_AP1R0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Alternate SGI Generation Register 1 */

static inline void aarch64_write_icc_asgi1r_el1(uint64_t d)
{
    __asm volatile("msr ICC_ASGI1R_EL1, %0" ::"r"(d) : "memory");
}

/* Binary Point Register 0 */

static inline void aarch64_write_icc_bpr0_el1(uint32_t d)
{
    __asm volatile("msr ICC_BPR0_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_bpr0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_BPR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Binary Point Register 1 */

static inline void aarch64_write_icc_bpr1_el1(uint32_t d)
{
    __asm volatile("msr ICC_BPR1_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_bpr1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_BPR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Control Register for EL1 */

static inline void aarch64_write_icc_ctlr_el1(uint32_t d)
{
    __asm volatile("msr ICC_CTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_ctlr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_CTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Control Register for EL3 */

static inline void aarch64_write_icc_ctlr_el3(uint32_t d)
{
    __asm volatile("msr ICC_CTLR_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_ctlr_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_CTLR_EL3" : "=r"(d)::"memory");
    return d;
}

/* Deactivate Interrupt Register */

static inline void aarch64_write_icc_dir_el1(uint32_t d)
{
    __asm volatile("msr ICC_DIR_EL1, %0" ::"r"(d) : "memory");
}

/* End Of Interrupt Register 0 */

static inline void aarch64_write_icc_eoir0_el1(uint32_t d)
{
    __asm volatile("msr ICC_EOIR0_EL1, %0" ::"r"(d) : "memory");
}

/* End Of Interrupt Register 1 */

static inline void aarch64_write_icc_eoir1_el1(uint32_t d)
{
    __asm volatile("msr ICC_EOIR1_EL1, %0" ::"r"(d) : "memory");
}

/* Highest Priority Pending Interrupt Register 0 */

static inline uint32_t aarch64_read_icc_hppir0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_HPPIR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Highest Priority Pending Interrupt Register 1 */

static inline uint32_t aarch64_read_icc_hppir1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_HPPIR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Acknowledge Register 0 */

static inline uint32_t aarch64_read_icc_iar0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_IAR0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Acknowledge Register 1 */

static inline uint32_t aarch64_read_icc_iar1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_IAR1_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Group Enable Register 0 */

static inline void aarch64_write_icc_igrpen0_el1(uint32_t d)
{
    __asm volatile("msr ICC_IGRPEN0_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_igrpen0_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_IGRPEN0_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Group Enable Register 1 */

static inline void aarch64_write_icc_igrpen1_el1(uint32_t d)
{
    __asm volatile("msr ICC_IGRPEN1_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_igrpen1_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_IGRPEN1_EL1" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Group Enable Register 1 for EL3 */

static inline void aarch64_write_icc_igrpen1_el3(uint32_t d)
{
    __asm volatile("msr ICC_IGRPEN1_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_igrpen1_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_IGRPEN1_EL3" : "=r"(d)::"memory");
    return d;
}

/* Priority Mask Register */

static inline void aarch64_write_icc_pmr_el1(uint32_t d)
{
    __asm volatile("msr ICC_PMR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_pmr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_PMR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Running Priority Register */

static inline uint32_t aarch64_read_icc_rpr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_RPR_EL1" : "=r"(d)::"memory");
    return d;
}

/* SGI Generation Register 0 */

static inline void aarch64_write_icc_sgi0r_el1(uint64_t d)
{
    __asm volatile("msr ICC_SGI0R_EL1, %0" ::"r"(d) : "memory");
}

/* SGI Generation Register 1 */

static inline void aarch64_write_icc_sgi1r_el1(uint64_t d)
{
    __asm volatile("msr ICC_SGI1R_EL1, %0" ::"r"(d) : "memory");
}

/* System Register Enable Register for EL1 */

static inline void aarch64_write_icc_sre_el1(uint32_t d)
{
    __asm volatile("msr ICC_SRE_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_sre_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_SRE_EL1" : "=r"(d)::"memory");
    return d;
}

/* System Register Enable Register for EL2 */

static inline void aarch64_write_icc_sre_el2(uint32_t d)
{
    __asm volatile("msr ICC_SRE_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_sre_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_SRE_EL2" : "=r"(d)::"memory");
    return d;
}

/* System Register Enable Register for EL3 */

static inline void aarch64_write_icc_sre_el3(uint32_t d)
{
    __asm volatile("msr ICC_SRE_EL3, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_icc_sre_el3(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICC_SRE_EL3" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller Hyp Active Priorities Register (0 0) */

static inline void aarch64_write_ich_ap0r0_el2(uint32_t d)
{
    __asm volatile("msr ICH_AP0R0_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_ich_ap0r0_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_AP0R0_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller Hyp Active Priorities Register (1 0) */

static inline void aarch64_write_ich_ap1r0_el2(uint32_t d)
{
    __asm volatile("msr ICH_AP1R0_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_ich_ap1r0_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_AP1R0_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller End of Interrupt Status Register */

static inline uint32_t aarch64_read_ich_eisr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_EISR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller Empty List Register Status Register */

static inline uint32_t aarch64_read_ich_elrsr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_ELRSR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller Hyp Control Register */

static inline void aarch64_write_ich_hcr_el2(uint32_t d)
{
    __asm volatile("msr ICH_HCR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_ich_hcr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_HCR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller List Register 0 */

static inline void aarch64_write_ich_lr0_el2(uint64_t d)
{
    __asm volatile("msr ICH_LR0_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ich_lr0_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ICH_LR0_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller List Register 1 */

static inline void aarch64_write_ich_lr1_el2(uint64_t d)
{
    __asm volatile("msr ICH_LR1_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ich_lr1_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ICH_LR1_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller List Register 2 */

static inline void aarch64_write_ich_lr2_el2(uint64_t d)
{
    __asm volatile("msr ICH_LR2_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ich_lr2_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ICH_LR2_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller List Register 3 */

static inline void aarch64_write_ich_lr3_el2(uint64_t d)
{
    __asm volatile("msr ICH_LR3_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_ich_lr3_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, ICH_LR3_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller Maintenance Interrupt State Register */

static inline uint32_t aarch64_read_ich_misr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_MISR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller Virtual Machine Control Register */

static inline void aarch64_write_ich_vmcr_el2(uint32_t d)
{
    __asm volatile("msr ICH_VMCR_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_ich_vmcr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_VMCR_EL2" : "=r"(d)::"memory");
    return d;
}

/* Interrupt Controller VGIC Type Register */

static inline uint32_t aarch64_read_ich_vtr_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, ICH_VTR_EL2" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 Generic Timer registers
 */

/* Counter-timer Kernel Control register */

static inline void aarch64_write_cntkctl_el1(uint32_t d)
{
    __asm volatile("msr CNTKCTL_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntkctl_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTKCTL_EL1" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Frequency register */

static inline void aarch64_write_cntfrq_el0(uint32_t d)
{
    __asm volatile("msr CNTFRQ_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntfrq_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTFRQ_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Physical Count register */

static inline uint64_t aarch64_read_cntpct_el0(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CNTPCT_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Virtual Count register */

static inline uint64_t aarch64_read_cntvct_el0(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CNTVCT_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Physical Timer TimerValue register */

static inline void aarch64_write_cntp_tval_el0(uint32_t d)
{
    __asm volatile("msr CNTP_TVAL_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntp_tval_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTP_TVAL_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Physical Timer Control register */

static inline void aarch64_write_cntp_ctl_el0(uint32_t d)
{
    __asm volatile("msr CNTP_CTL_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntp_ctl_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTP_CTL_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Physical Timer CompareValue register */

static inline void aarch64_write_cntp_cval_el0(uint64_t d)
{
    __asm volatile("msr CNTP_CVAL_EL0, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cntp_cval_el0(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CNTP_CVAL_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Virtual Timer TimerValue register */

static inline void aarch64_write_cntv_tval_el0(uint32_t d)
{
    __asm volatile("msr CNTV_TVAL_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntv_tval_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTV_TVAL_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Virtual Timer Control register */

static inline void aarch64_write_cntv_ctl_el0(uint32_t d)
{
    __asm volatile("msr CNTV_CTL_EL0, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntv_ctl_el0(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTV_CTL_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Virtual Timer CompareValue register */

static inline void aarch64_write_cntv_cval_el0(uint64_t d)
{
    __asm volatile("msr CNTV_CVAL_EL0, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cntv_cval_el0(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CNTV_CVAL_EL0" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Virtual Offset register */

static inline void aarch64_write_cntvoff_el2(uint64_t d)
{
    __asm volatile("msr CNTVOFF_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cntvoff_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CNTVOFF_EL2" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Hypervisor Control register */

static inline void aarch64_write_cnthctl_el2(uint32_t d)
{
    __asm volatile("msr CNTHCTL_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cnthctl_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTHCTL_EL2" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Hypervisor Physical Timer TimerValue register */

static inline void aarch64_write_cnthp_tval_el2(uint32_t d)
{
    __asm volatile("msr CNTHP_TVAL_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cnthp_tval_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTHP_TVAL_EL2" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Hypervisor Physical Timer Control register */

static inline void aarch64_write_cnthp_ctl_el2(uint32_t d)
{
    __asm volatile("msr CNTHP_CTL_EL2, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cnthp_ctl_el2(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTHP_CTL_EL2" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Hypervisor Physical Timer CompareValue register */

static inline void aarch64_write_cnthp_cval_el2(uint64_t d)
{
    __asm volatile("msr CNTHP_CVAL_EL2, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cnthp_cval_el2(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CNTHP_CVAL_EL2" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Physical Secure Timer TimerValue register */

static inline void aarch64_write_cntps_tval_el1(uint32_t d)
{
    __asm volatile("msr CNTPS_TVAL_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntps_tval_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTPS_TVAL_EL1" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Physical Secure Timer Control register */

static inline void aarch64_write_cntps_ctl_el1(uint32_t d)
{
    __asm volatile("msr CNTPS_CTL_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_cntps_ctl_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, CNTPS_CTL_EL1" : "=r"(d)::"memory");
    return d;
}

/* Counter-timer Physical Secure Timer CompareValue register */

static inline void aarch64_write_cntps_cval_el1(uint64_t d)
{
    __asm volatile("msr CNTPS_CVAL_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cntps_cval_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CNTPS_CVAL_EL1" : "=r"(d)::"memory");
    return d;
}

/*
 * AArch64 implementation defined registers
 */

/* L2 Control Register */

static inline void aarch64_write_l2ctlr_el1(uint32_t d)
{
    __asm volatile("msr L2CTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_l2ctlr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, L2CTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* L2 Extended Control Register */

static inline void aarch64_write_l2ectlr_el1(uint32_t d)
{
    __asm volatile("msr L2ECTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_l2ectlr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, L2ECTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* L2 Auxiliary Control Register */

static inline void aarch64_write_l2actlr_el1(uint32_t d)
{
    __asm volatile("msr L2ACTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint32_t aarch64_read_l2actlr_el1(void)
{
    uint32_t d;
    __asm volatile("mrs %0, L2ACTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* CPU Auxiliary Control Register */

static inline void aarch64_write_cpuactlr_el1(uint64_t d)
{
    __asm volatile("msr CPUACTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cpuactlr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CPUACTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* CPU Extended Control Register */

static inline void aarch64_write_cpuectlr_el1(uint64_t d)
{
    __asm volatile("msr CPUECTLR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cpuectlr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CPUECTLR_EL1" : "=r"(d)::"memory");
    return d;
}

/* CPU Memory Error Syndrome Register */

static inline void aarch64_write_cpumerrsr_el1(uint64_t d)
{
    __asm volatile("msr CPUMERRSR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_cpumerrsr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CPUMERRSR_EL1" : "=r"(d)::"memory");
    return d;
}

/* L2 Memory Error Syndrome Register */

static inline void aarch64_write_l2merrsr_el1(uint64_t d)
{
    __asm volatile("msr L2MERRSR_EL1, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_l2merrsr_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, L2MERRSR_EL1" : "=r"(d)::"memory");
    return d;
}

/* Configuration Base Address Register */

static inline uint64_t aarch64_read_cbar_el1(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CBAR_EL1" : "=r"(d)::"memory");
    return d;
}

/*
 * Part of PSTATE
 */

/* SPSel */

static inline void aarch64_write_spsel(uint64_t d)
{
    __asm volatile("msr SPSel, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_spsel(void)
{
    uint64_t d;
    __asm volatile("mrs %0, SPSel" : "=r"(d)::"memory");
    return d;
}

/* CurrentEL */

static inline uint64_t aarch64_read_currentel(void)
{
    uint64_t d;
    __asm volatile("mrs %0, CurrentEL" : "=r"(d)::"memory");
    return d;
}

/* DAIF */

static inline void aarch64_write_daif(uint64_t d)
{
    __asm volatile("msr DAIF, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_daif(void)
{
    uint64_t d;
    __asm volatile("mrs %0, DAIF" : "=r"(d)::"memory");
    return d;
}

/* NZCV */

static inline void aarch64_write_nzcv(uint64_t d)
{
    __asm volatile("msr NZCV, %0" ::"r"(d) : "memory");
}

static inline uint64_t aarch64_read_nzcv(void)
{
    uint64_t d;
    __asm volatile("mrs %0, NZCV" : "=r"(d)::"memory");
    return d;
}

/* system register APIs in current exception level */

void aarch64_write_mair(uint64_t d);
uint64_t aarch64_read_mair(void);
void aarch64_write_amair(uint64_t d);
uint64_t aarch64_read_amair(void);
void aarch64_write_tcr(uint64_t d);
uint64_t aarch64_read_tcr(void);
void aarch64_write_ttbr0(uint64_t d);
uint64_t aarch64_read_ttbr0(void);
void aarch64_write_sctlr(uint64_t d);
uint64_t aarch64_read_sctlr(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_H */
