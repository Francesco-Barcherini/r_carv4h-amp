#include <stddef.h>
#include "serial.h"
#include "cbfw_printf.h"
#include "memory.h"

/*******************************************************************************
 * Copyright (c) 2018 Renesas Electronics Corporation. All rights reserved.
 *
 * DESCRIPTION   : SCIF driver
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "scif_register.h"
//#include <rcar_def.h>
#include "serial.h"
//#include <mem_io.h>
//#include <micro_wait.h>
//#include <rst_register.h>
#include "scif.h"

extern void memory_main();

int main(void)
{
	scif_init();
	memory_main();
}