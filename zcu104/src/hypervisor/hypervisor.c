/*
 * hypervisor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"

/* defines */

/* types */

/* prototypes */

void *main_stack_top(void);

/* variables */

/* functions */

static void init_exception(void)
{
    extern char excvec_hyp[];

    aarch64_write_vbar_el2((uint64_t)excvec_hyp);
    aarch64_isb();
}

void hypervisor(void)
{
    void test_guest_01(void);
    void *sp;

    init_exception();
    sp = main_stack_top();
    thread_launch((void *)test_guest_01, sp, NULL);
}

