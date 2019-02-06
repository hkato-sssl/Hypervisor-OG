/*
 * driver/aarch64/exception.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_EXCEPTION_H
#define DRIVER_AARCH64_EXCEPTION_H

/* includes */

/* defines */

/* Exception Information */

#define EXC_VECTOR      0
#define EXC_ESR         1
#define EXC_IFSR32      2
#define EXC_FAR         3
#define EXC_SPSR        4
#define EXC_ELR         5
#define EXC_SP0         6
#define EXC_SP1         7
#define EXC_X0          6
#define EXC_X1          (EXC_X0 + 1)
#define EXC_X2          (EXC_X0 + 2)            
#define EXC_X3          (EXC_X0 + 3)            
#define EXC_X4          (EXC_X0 + 4)            
#define EXC_X5          (EXC_X0 + 5)            
#define EXC_X6          (EXC_X0 + 6)            
#define EXC_X7          (EXC_X0 + 7)            
#define EXC_X8          (EXC_X0 + 8)            
#define EXC_X9          (EXC_X0 + 9)            
#define EXC_X10         (EXC_X0 + 10)
#define EXC_X11         (EXC_X0 + 11)           
#define EXC_X12         (EXC_X0 + 12)           
#define EXC_X13         (EXC_X0 + 13)           
#define EXC_X14         (EXC_X0 + 14)           
#define EXC_X15         (EXC_X0 + 15)           
#define EXC_X16         (EXC_X0 + 16)           
#define EXC_X17         (EXC_X0 + 17)           
#define EXC_X18         (EXC_X0 + 18)           
#define EXC_X19         (EXC_X0 + 19)           
#define EXC_X20         (EXC_X0 + 20)
#define EXC_X21         (EXC_X0 + 21)           
#define EXC_X22         (EXC_X0 + 22)           
#define EXC_X23         (EXC_X0 + 23)           
#define EXC_X24         (EXC_X0 + 24)           
#define EXC_X25         (EXC_X0 + 25)           
#define EXC_X26         (EXC_X0 + 26)           
#define EXC_X27         (EXC_X0 + 27)           
#define EXC_X28         (EXC_X0 + 28)           
#define EXC_X29         (EXC_X0 + 29)           
#define EXC_X30         (EXC_X0 + 30)           
#define EXC_DUMMY       (EXC_X0 + 31)   /* for 16-byte alignment */

#define	NR_EXC_REGS	    (EXC_DUMMY + 1)

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

const char *aarch64_esr_ec_message(uint32_t ec);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_EXCEPTION_H */

