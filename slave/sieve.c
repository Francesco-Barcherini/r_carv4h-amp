#include <stddef.h>
#include "serial.h"
#include "cbfw_printf.h"

void (*monHook)(void) __attribute__ ((section (".data"))) = 0;
extern void (*watchdogTrigger)(void);

int main(void)
{
	uart_rcar_configure();
	bprintf(WELCOME_STRING);
}