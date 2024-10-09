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
	// try to write 0x100 every 0x100000 from 0 to 0xEFFFFFFF (avoid 0x40000000-0x4FFFFFFF addresses)
	// read value, then write 0x100, then read again and verify that it is changed to 0x100
	// print for every address if the test is successful
	// if last 8 writes are successful, print the first address (we won!)
	uint64_t start 	= 0x00000000;
	uint64_t end 	= 0xFFFFFFFF;
	for (uint64_t i = start; i < end; i += 0x100000) {
		if (i >= 0x40000000 && i < 0xE0000000) {
			continue;
		}
		// EB000000-EBFFFFFF is reserved for the SCIF
		if (i >= 0xEB000000 && i < 0xEBFFFFFF) {
			continue;
		}

		uint64_t* ptr = (uint64_t*)i;
		uint64_t val = *ptr;
		*ptr = 0x100;
		uint64_t val2 = *ptr;
		if (val2 == 0x100) {
			bprintf("Address 0x%x: Success\r\n", i);
		} else {
			bprintf("Address 0x%x: Fail\r\n", i);
		}
	}
	memory_main();
}