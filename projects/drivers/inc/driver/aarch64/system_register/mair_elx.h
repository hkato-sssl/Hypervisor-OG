/*
 * driver/aarch64/system_register/mair_elx.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_MAIR_ELX_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_MAIR_ELX_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

#define NR_MAIR_ATTRS             8

#define MAIR_ATTR(n, v)           (((v)&0xffULL) << ((n)*8))

/* Device memory */
#define MAIR_ATTR_DEVICE_nGnRnE   0x00
#define MAIR_ATTR_DEVICE_nGnRE    0x04
#define MAIR_ATTR_DEVICE_nGRE     0x08
#define MAIR_ATTR_DEVICE_GRE      0x0c

/* Normal Memory, Outer Write-through transient */
#define MAIR_ATTR_NORMAL_OWTTWA   0x10
#define MAIR_ATTR_NORMAL_OWTTRA   0x20
#define MAIR_ATTR_NORMAL_OWTTRAWA 0x30

/* Normal Memory, Outer Non-cacheable */
#define MAIR_ATTR_NORMAL_ONC      0x40

/* Normal Memory, Outer Write-back transient */
#define MAIR_ATTR_NORMAL_OWBTWA   0x50
#define MAIR_ATTR_NORMAL_OWBTRA   0x60
#define MAIR_ATTR_NORMAL_OWBTRAWA 0x70

/* Normal Memory, Outer Write-through non-transient */
#define MAIR_ATTR_NORMAL_OWTWA    0x90
#define MAIR_ATTR_NORMAL_OWTRA    0xa0
#define MAIR_ATTR_NORMAL_OWTRAWA  0xb0

/* Normal Memory, Outer Write-back non-transient */
#define MAIR_ATTR_NORMAL_OWBWA    0xd0
#define MAIR_ATTR_NORMAL_OWBRA    0xe0
#define MAIR_ATTR_NORMAL_OWBRAWA  0xf0

/* Normal Memory, Inner Write-through transient */
#define MAIR_ATTR_NORMAL_IWTTWA   0x01
#define MAIR_ATTR_NORMAL_IWTTRA   0x02
#define MAIR_ATTR_NORMAL_IWTTRAWA 0x03

/* Normal Memory, Inner Non-cacheable */
#define MAIR_ATTR_NORMAL_INC      0x04

/* Normal Memory, Inner Write-back transient */
#define MAIR_ATTR_NORMAL_IWBTWA   0x05
#define MAIR_ATTR_NORMAL_IWBTRA   0x06
#define MAIR_ATTR_NORMAL_IWBTRAWA 0x07

/* Normal Memory, Inner Write-through non-transient */
#define MAIR_ATTR_NORMAL_IWTWA    0x09
#define MAIR_ATTR_NORMAL_IWTRA    0x0a
#define MAIR_ATTR_NORMAL_IWTRAWA  0x0b

/* Normal Memory, Inner Write-back non-transient */
#define MAIR_ATTR_NORMAL_IWBWA    0x0d
#define MAIR_ATTR_NORMAL_IWBRA    0x0e
#define MAIR_ATTR_NORMAL_IWBRAWA  0x0f

#define MAIR_ATTR_NORMAL_NC       (MAIR_ATTR_NORMAL_ONC | MAIR_ATTR_NORMAL_INC)
#define MAIR_ATTR_NORMAL_WT       (MAIR_ATTR_NORMAL_OWTRA | MAIR_ATTR_NORMAL_IWTRA)
#define MAIR_ATTR_NORMAL_WTWA \
    (MAIR_ATTR_NORMAL_OWTRAWA | MAIR_ATTR_NORMAL_IWTRAWA)
#define MAIR_ATTR_NORMAL_WB (MAIR_ATTR_NORMAL_OWBRA | MAIR_ATTR_NORMAL_IWBRA)
#define MAIR_ATTR_NORMAL_WBWA \
    (MAIR_ATTR_NORMAL_OWBRAWA | MAIR_ATTR_NORMAL_IWBRAWA)
/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_MAIR_ELX_H */
