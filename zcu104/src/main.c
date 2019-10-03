/*
 * main.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

void init_system(void);
void init_hw(void);
void launch_system(void);

int main(void)
{
	init_system();
	init_hw();
	launch_system();

	return (0);
}

