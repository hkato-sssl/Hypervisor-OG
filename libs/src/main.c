/*
 * main.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/init.h"
#include "lib/system/printk.h"

/* defines */

/* variables */

/* functions */

int main(void)
{
	init_printk();

	printk("----+----+\n");
	printk("%10hhd\n", 10);
	printk("%+10hhd\n", 10);
	printk("%10hi\n", 300000);
	printk("%10i\n", 300000);
	printk("%10hhi\n", 200);

	return 1;
}
