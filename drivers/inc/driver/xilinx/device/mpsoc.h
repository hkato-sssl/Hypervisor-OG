/*
 * driver/xilinx/device/mpsoc.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_XILINX_DEVICE_MPSOC_H
#define DRIVER_XILINX_DEVICE_MPSOC_H

/* includes */

/* defines */

#define NR_SOC_CPUS         4
#define REG_GIC400D         0xf9010000
#define REG_GIC400C         0xf9020000
#define REG_GIC400H         0xf9040000
#define REG_GIC400V         0xf9060000
#define REG_SMMU500         0xfd800000

#define REG_UART0           0xff000000
#define REG_UART1           0xff010000

#endif /* DRIVER_XILINX_DEVICE_MPSOC_H */

