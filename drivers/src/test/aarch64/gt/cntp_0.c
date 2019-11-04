/*
 * test/aarch64/gt/cntp_0.c - test of EL1 physical timer
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "driver/test/aarch64/gt.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void dump_cntp(void)
{
	uintptr_t tv, cv, ctl;

	aarch64_isb();
	tv = aarch64_read_cntp_tval_el0();
	cv = aarch64_read_cntp_cval_el0();
	ctl = aarch64_read_cntp_ctl_el0();
	printk("CNTP_TVAL_EL0: %lu\n", tv);
	printk("CNTP_CVAL_EL0: %llu\n", cv);
	printk(" CNTP_CTL_EL0: 0x%04x\n", ctl);
	printk("\n");
}

void test_gt_cntp_0(void)
{
	printk("<%s>\n", __func__);
	dump_cntp();
	aarch64_write_cntp_tval_el0(1000000000UL);
	aarch64_isb();
	dump_cntp();
	aarch64_write_cntp_tval_el0(1000000000UL);
	aarch64_isb();
	dump_cntp();
}

