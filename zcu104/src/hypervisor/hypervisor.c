/*
 * hypervisor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/tls.h"

/* defines */

/* types */

/* prototypes */

void *exc_stack_top(void);

/* variables */

/* functions */

static void init_tls(void)
{
    char *sp;
    uint64_t *tls;

    sp = exc_stack_top();
    sp -= TLS_SIZE;

    tls = (uint64_t *)sp;
    memset(tls, 0, TLS_SIZE);
    tls[TLS_EXCEPTION_SP] = (uint64_t)sp;

    aarch64_write_tpidr_el2((uint64_t)tls);
}

static void init_exception(void)
{
    extern char excvec_hyp[];

    aarch64_write_vbar_el2((uint64_t)excvec_hyp);
    aarch64_isb();
}

void hypervisor(void)
{
    init_tls();
    init_exception();

    test_vm_01_launch();
}

