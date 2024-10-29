// SPDX-License-Identifier: Apache-2.0
/*
 * test/vspi/vspi.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef TEST_VSPI_VSPI_H
#define TEST_VSPI_VSPI_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

void vspi_set_start(bool);
bool vspi_get_start(void);
void vspi_init_interrupts(uint8_t level);
void vspi_assert_interrupt(uint32_t no);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* TEST_VSPI_VSPI_H */
