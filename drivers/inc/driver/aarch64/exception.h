/*
 * driver/aarch64/exception.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_EXCEPTION_H
#define DRIVER_AARCH64_EXCEPTION_H

/* includes */

/* defines */

#ifndef ASSEMBLER

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

#endif /* ASSEMBLER */

#endif /* DRIVER_AARCH64_EXCEPTION_H */

