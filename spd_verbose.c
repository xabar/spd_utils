/*
 * spd_verbose.c
 *
 *  Created on: 09.11.2018
 *      Author: Perepelitsin Roman
 */

#include <stdio.h>
#include "spd_verbose.h"


int verbose_level = 0;

void set_verbose_level(int vl)
{
	verbose_level = vl;
}

