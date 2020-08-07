/*
 * hypervisor/parameter.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_PARAMETER_H
#define HYPERVISOR_PARAMETER_H

/* includes */

/* defines */

#define HYP_MAX_NR_VPCS             8

#define HYP_KERNEL_STACK_SIZE       (4 * 1024)

#define HYP_INTR_VPC_EVENT          7

#define HYP_GUEST_REGION_BASE       0xf00000000000ULL

#endif /* HYPERVISOR_PARAMETER_H */

