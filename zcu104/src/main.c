/*
 * main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t init_system(void);
void init_hw(void);
void launch_system(void);

int main(void)
{
	init_system();
	init_hw();
	launch_system();

	return (0);
}

void init_hw(void) { return ; }
