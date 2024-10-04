#include <stddef.h>
#include "serial.h"
#include "cbfw_printf.h"

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

// /* Define  */
// #define	SCIF_SCSCR_TE_EN	(uint16_t)((uint16_t)1U << 5U)
// #define	SCIF_SCSCR_RE_EN	(uint16_t)((uint16_t)1U << 4U)
// #define	SCIF_SCSCR_CKE_EXT_CLK	(uint16_t)((uint16_t)2U << 0U)
// #define SCIF_SCSCR_INIT_DATA	(uint16_t)(SCIF_SCSCR_TE_EN | SCIF_SCSCR_RE_EN)
// #define SCIF_SCSCR_HW_INIT	(uint16_t)(0x0000U)

// #define	SCIF_SCFCR_TFRST_EN	(uint16_t)((uint16_t)1U << 2U)
// #define	SCIF_SCFCR_RFRS_EN	(uint16_t)((uint16_t)1U << 1U)
// #define SCIF_SCFCR_RESET_FIFO	(uint16_t)(SCIF_SCFCR_TFRST_EN | SCIF_SCFCR_RFRS_EN)
// #define SCIF_SCFCR_INIT_DATA	(uint16_t)(0x0000U)

// #define SCIF_SCFSR_TEND		(uint16_t)((uint16_t)1U << 6U)
// #define SCIF_SCFSR_TDFE		(uint16_t)((uint16_t)1U << 5U)
// #define TRANS_END_CHECK		(uint16_t)(SCIF_SCFSR_TEND | SCIF_SCFSR_TDFE)
// #define SCIF_SCFSR_INIT_DATA	(uint16_t)(0x0000U)

// #define SCIF_SCLSR_INIT_DATA	(uint16_t)(0x0000U)

// #define SCIF_SCSMR_INIT_DATA	(uint16_t)(0x0000U)

// /* Pclk(66MHz)/1, 115.2kBps*/
// /* N = 66/(66/2*115200)*10^4-1 =17=> 0x11 */
// #define	SCIF_SCBRR_115200BPS	(uint8_t)(0x11U)
// /* Pclk(266MHz)/1, 921.6kBps*/
// /* N = 266/(8*2*921600)*10^6-1 =17=> 0x11 */
// #define	HSCIF_SCBRR_921600BPS	(uint8_t)(0x11U)
// /* Pclk(266MHz)/1, 1.8432MBps*/
// /* N = 266/(8*2*1843200)*10^6-1 =8=> 0x08 */
// #define	HSCIF_SCBRR_1843200BPS	(uint8_t)(0x08U)
// #define SCIF_SCBRR_HW_INIT	(uint8_t)(0xFFU)

// #define HSCIF_HSSRR_SRE		(uint16_t)(1U << 15U)
// #define HSCIF_HSSRR_SRCYC8	(uint16_t)(7U << 0U)	/* Sampling rate 8-1 */
// #define HSCIF_HSSRR_VAL		(uint16_t)(HSCIF_HSSRR_SRE | HSCIF_HSSRR_SRCYC8)

// #define HSCIF_DL_DIV1		(uint16_t)(1U << 0U)
// #define HSCIF_CKS_SC_CLK_EXT	(uint16_t)(0x0000U)


// #define MODEMR_SCIF_DLMODE		(0x00000000U)
// #define MODEMR_HSCIF_DLMODE_921600	(0x00000001U)
// #define MODEMR_HSCIF_DLMODE_1843200	(0x00000002U)
// #define MODEMR_HSCIF_DLMODE_3000000	(0x00000003U)

// static void (*put_char)(uint8_t);

// static void scif_console_putc(uint8_t outchar);
// static void hscif_console_putc(uint8_t outchar);

// static void mem_write16(uint32_t addr, uint16_t value)
// {
// 	*((uint16_t *) addr) = value;
// }

// void scif_init(void)
// {
//     /* clear SCR.TE & SCR.RE*/
//     mem_write16(SCIF_SCSCR_0, SCIF_SCSCR_HW_INIT);
//     /* reset tx-fifo, reset rx-fifo. */
//     mem_write16(SCIF_SCFCR_0, SCIF_SCFCR_RESET_FIFO);

//     /* clear ORER bit */
//     mem_write16(SCIF_SCLSR_0, SCIF_SCLSR_INIT_DATA);
//     /* clear all error bit	*/
//     mem_write16(SCIF_SCFSR_0, SCIF_SCFSR_INIT_DATA);

//     /* internal clock, SC_CLK pin unused for output pin */
//     mem_write16(SCIF_SCSCR_0, SCIF_SCSCR_HW_INIT);
//     /* 8bit data, no-parity, 1 stop, Po/1 */
//     mem_write16(SCIF_SCSMR_0, SCIF_SCSMR_INIT_DATA);

//     /* Baud rate 115200bps*/
//     mem_write16(SCIF_SCBRR_0, SCIF_SCBRR_115200BPS);

//     for (int i = 0; i < 100000; i++)
//         ;

//     /* reset-off tx-fifo, rx-fifo. */
//     mem_write16(SCIF_SCFCR_0, SCIF_SCFCR_INIT_DATA);
//     /* enable TE, RE; SC_CLK=no output */
//     mem_write16(SCIF_SCSCR_0, SCIF_SCSCR_INIT_DATA);

// }

void (*monHook)(void) __attribute__ ((section (".data"))) = 0;
extern void (*watchdogTrigger)(void);

int main(void)
{
	//uart_rcar_configure();
	scif_init();
	while(1)
		bprintf(WELCOME_STRING);
}