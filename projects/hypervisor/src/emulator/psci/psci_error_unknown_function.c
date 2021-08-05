/*
 * emulator/psci/psci_error_unknown_function.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/emulator/psci.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t psci_error_unknown_function(struct vpc *vpc)
{
    /*
     * Ref.: SMC Calling Convention (Issue B)
     *       5.2 Unknown Function Identifier
     */
    vpc->regs[VPC_X0] = BITS(63, 0);

    return SUCCESS;
}
