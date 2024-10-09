/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 * Copyright 2021-2023 Renesas Electronics Corporation All rights reserved.
 *******************************************************************************/

/*******************************************************************************
 * DESCRIPTION   : SCIF driver
 ******************************************************************************/
/******************************************************************************
 * @file          scif.c
 * - Version      : 0.08
 * @brief         1. Initial setting of SCIF.
 *                2. Initial setting of HSCIF.
 *                3. Log output function.
 * .
 *****************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.07.2021 0.01     First Release
 *         : 03.09.2021 0.02     Modify the timing of MODEMR judgement.
 *         : 15.10.2021 0.03     Modify register access to read modify write.
 *         : 03.12.2021 0.04     Fix incorrect configuration process.
 *         : 06.01.2022 0.05     Static analysis support
 *         : 23.05.2022 0.06     Integration of S4 and V4H
 *         : 20.12.2022 0.07     Modify writing bit size to SCBRR register.
 *         : 21.08.2023 0.08     Add support for V4M.
 *****************************************************************************/

#include <stdint.h>
//#include <types.h>
#include <cpg.h>
#include <pfc.h>
#include <scif.h>
#include <mem_io.h>
#include <rst_register.h>


/* Define  */
#define SCIF_SCSCR_TE_EN        (uint16_t)((uint16_t)1U << 5U)
#define SCIF_SCSCR_RE_EN        (uint16_t)((uint16_t)1U << 4U)
#define SCIF_SCSCR_CKE_EXT_CLK  (uint16_t)((uint16_t)2U << 0U)
#define SCIF_SCSCR_INIT_DATA    (uint16_t)(SCIF_SCSCR_TE_EN | SCIF_SCSCR_RE_EN)
#define SCIF_SCSCR_HW_INIT      (uint16_t)(0x0000U)

#define SCIF_SCFCR_TFRST_EN     (uint16_t)((uint16_t)1U << 2U)
#define SCIF_SCFCR_RFRS_EN      (uint16_t)((uint16_t)1U << 1U)
#define SCIF_SCFCR_RESET_FIFO   (uint16_t)(SCIF_SCFCR_TFRST_EN | SCIF_SCFCR_RFRS_EN)

#define SCIF_SCFSR_TEND         (uint16_t)((uint16_t)1U << 6U)
#define SCIF_SCFSR_TDFE         (uint16_t)((uint16_t)1U << 5U)
#define TRANS_END_CHECK         (uint16_t)(SCIF_SCFSR_TEND | SCIF_SCFSR_TDFE)
#define SCIF_SCFSR_INIT_DATA    (uint16_t)(0x0000U)

#define SCIF_SCLSR_INIT_DATA    (uint16_t)(0x0000U)

#define SCIF_SCSMR_CHR          (uint16_t)((uint16_t)1U << 6U)
#define SCIF_SCSMR_PE           (uint16_t)((uint16_t)1U << 5U)
#define SCIF_SCSMR_STOP         (uint16_t)((uint16_t)1U << 3U)
#define SCIF_SCSMR_CKS          (uint16_t)((uint16_t)3U << 0U)
#define SCIF_SCSMR_INIT_DATA    ~((uint16_t)(SCIF_SCSMR_CHR | SCIF_SCSMR_PE | SCIF_SCSMR_STOP | SCIF_SCSMR_CKS))

/* Pclk(66MHz)/1, 115.2kbps*/
/* N = 66/(66/2*115200)*10^4-1 =17=> 0x11 */
#define SCIF_SCBRR_115200BPS    (uint8_t)(0x11U)
/* Pclk(266MHz)/1, 921.6kbps*/
/* N = 266/(8*2*921600)*10^6-1 =17=> 0x11 */
#define HSCIF_SCBRR_921600BPS   (uint8_t)(0x11U)
/* Pclk(266MHz)/1, 1.8432Mbps*/
/* N = 266/(8*2*1843200)*10^6-1 =8=> 0x08 */
#define HSCIF_SCBRR_1843200BPS  (uint8_t)(0x08U)

#define HSCIF_HSSRR_SRE         (uint16_t)(1U << 15U)
#define HSCIF_HSSRR_SRCYC       (uint16_t)(0x1FU << 0U)
#define HSCIF_HSSRR_SRCYC8      (uint16_t)(7U << 0U)    /* Sampling rate 8-1 */
#define HSCIF_HSSRR_VAL         (uint16_t)(HSCIF_HSSRR_SRE | HSCIF_HSSRR_SRCYC8)

#define HSCIF_DL_DIV1           (uint16_t)(1U << 0U)
#define HSCIF_CKS_CKS           (uint16_t)(1U << 15U)
#define HSCIF_CKS_XIN           (uint16_t)(1U << 14U)
#define HSCIF_CKS_SC_CLK_EXT    ~((uint16_t)(HSCIF_CKS_CKS | HSCIF_CKS_XIN))

#define RCAR_LSI RCAR_V4H

#define CPG_MSTPCR_HSCIF        (((uint32_t)1U) << 14U)
#define CPG_MSTPCR_SCIF         (((uint32_t)1U) << 2U)

#define GPSR_TX             ((uint32_t)1U   << 12U)                   /* HTX0 / TX0 */
#define GPSR_RX             ((uint32_t)1U   << 16U)                   /* HRX0 / RX0 */
#define IPSR_RX_VAL         ((uint32_t)1U   << 0U)                    /* RX0 */
#define IPSR_TX_VAL         ((uint32_t)1U   << 16U)                   /* TX0 */
#define POC_TX_33V          ((uint32_t)1U   << 12U)                   /* HTX0 / TX0 3.3V setting value */
#define POC_RX_33V          ((uint32_t)1U   << 16U)                   /* HRX0 / RX0 3.3V setting value */
#define POC_TX_18V          ((uint32_t)0U   << 12U)                   /* HTX0 / TX0 1.8V setting value */
#define POC_RX_18V          ((uint32_t)0U   << 16U)                   /* HRX0 / RX0 1.8V setting value */
#define IPSR_RX_MASK        ((uint32_t)0xFU << 0U)                    /* IPSR bit[3:0] */
#define IPSR_TX_MASK        ((uint32_t)0xFU << 16U)                   /* IPSR bit[19:16] */
#define PFC_GPSR_SCIF_MASK  (uint32_t)(0x00011000U)                   /* SCIF0/HSCIF0 RX/TX */
#define PFC_GPSR_SCIF_VAL   (uint32_t)(GPSR_TX | GPSR_RX)             /* SCIF0/HSCIF0 RX/TX */
#define PFC_IPSR_SCIF_MASK1 (uint32_t)(IPSR_TX_MASK)                  /* Mask value of IPSR (SCIF0/HSCIF0 TX) */
#define PFC_IPSR_SCIF_VAL1  (uint32_t)(IPSR_TX_VAL)                   /* SCIF0 TX */
#define PFC_IPSR_SCIF_MASK2 (uint32_t)(IPSR_RX_MASK)                  /* Mask value of IPSR (SCIF3/HSCIF0 RX) */
#define PFC_IPSR_SCIF_VAL2  (uint32_t)(IPSR_RX_VAL)                   /* SCIF0 RX */
#define PFC_IPSR_HSCIF_VAL1 (uint32_t)(0x00000000U)                   /* HSCIF0 TX */
#define PFC_IPSR_HSCIF_VAL2 (uint32_t)(0x00000000U)                   /* HSCIF0 RX */
#define PFC_POC_SCIF_MASK   (uint32_t)(0x00011000U)                   /* SCIF0/HSCIF0 RX/TX */
#define PFC_POC_SCIF_33V    (uint32_t)(POC_TX_33V | POC_RX_33V)       /* SCIF0/HSCIF0 RX/TX */
#define PFC_IPSR_SCIF_EXTCLK_MASK  (uint32_t)(0xFU << 4U)             /* Mask value of IPSR (External Clock) */
#define PFC_IPSR_SCIF_EXTCLK_VAL   (uint32_t)(0x0U << 4U)             /* IPSR (External Clock) */
#define PFC_GPSR_SCIF_EXTCLK_MASK  (uint32_t)(0x00020000U)            /* Mask value of IPSR (External Clock) */

static void (*rcar_putc)(uint8_t outchar);

static void scif_module_start();
static void scif_pfc_init();
static void scif_console_init();
static void scif_console_putc(uint8_t outchar);


static void scif_module_start()
{
    uint32_t reg;
    reg = mem_read32(CPG_MSTPSR5D0);
    /* If supply of clock to SCIF0 is stopped */
    if (0U != (CPG_MSTPCR_HSCIF & reg))
    {
        /* Supply of clock to SCIF0 is start */
        reg &= ~(CPG_MSTPCR_HSCIF);
        cpg_reg_write(CPG_MSTPCR5D0, CPG_MSTPSR5D0, reg);
    }

}
/* End of function scif_module_start(void) */

static void scif_pfc_init()
{
    uint32_t reg;
     /* Set HTX of HSCIF 0. */
    reg = mem_read32(PFC_IP1SR1_RW);
    reg &= (~(PFC_IPSR_SCIF_MASK1));
    reg |= PFC_IPSR_HSCIF_VAL1;
    pfc_reg_write(PFC_IP1SR1_RW, reg);

    /* Set HRX of HSCIF 0. */
    reg = mem_read32(PFC_IP2SR1_RW);
    reg &= (~(PFC_IPSR_SCIF_MASK2));
    reg |= PFC_IPSR_HSCIF_VAL2;
    pfc_reg_write(PFC_IP2SR1_RW, reg);

    /* Set Voltage setting of 3.3V. */
    reg = mem_read32(PFC_POC1_RW);
    reg &= (~(PFC_POC_SCIF_MASK));
    reg |= PFC_POC_SCIF_33V;
    pfc_reg_write(PFC_POC1_RW, reg);

    reg = mem_read32(PFC_GPSR1_RW);
    reg &= (~(PFC_GPSR_SCIF_MASK));
    reg |= PFC_GPSR_SCIF_VAL;
    pfc_reg_write(PFC_GPSR1_RW, reg);
}
/* End of function scif_pfc_init(void) */

static void scif_console_init()
{
    uint16_t reg;
    /* clear SCR.TE & SCR.RE*/
    mem_write16(HSCIF_HSSCR, SCIF_SCSCR_HW_INIT);
    /* reset tx-fifo, reset rx-fifo. */
    reg = mem_read16(HSCIF_HSFCR);
    reg |= SCIF_SCFCR_RESET_FIFO;
    mem_write16(HSCIF_HSFCR, reg);

    /* clear ORER bit */
    mem_write16(HSCIF_HSLSR, SCIF_SCLSR_INIT_DATA);
    /* clear all error bit  */
    mem_write16(HSCIF_HSFSR, SCIF_SCFSR_INIT_DATA);

    /* internal clock, SC_CLK pin unused for output pin */
    mem_write16(HSCIF_HSSCR, SCIF_SCSCR_HW_INIT);
    /* 8bit data, no-parity, 1 stop, Po/1 */
    reg = mem_read16(HSCIF_HSSMR);
    reg &= SCIF_SCSMR_INIT_DATA;
    mem_write16(HSCIF_HSSMR, reg);

    /* Sampling rate 8  */
    mem_write16(HSCIF_HSSRR, HSCIF_HSSRR_VAL);
    /* Baud rate 921600bps*/
    mem_write8(HSCIF_HSBRR, HSCIF_SCBRR_921600BPS);

    for (int i = 0; i < 100000; i++)
    {
        ;
    }

    /* reset-off tx-fifo, rx-fifo. */
    reg = mem_read16(HSCIF_HSFCR);
    reg &= ~(SCIF_SCFCR_RESET_FIFO);
    mem_write16(HSCIF_HSFCR, reg);
    /* enable TE, RE; SC_CLK=external */
    reg =  mem_read16(HSCIF_HSSCR);
    reg |= SCIF_SCSCR_INIT_DATA;
    mem_write16(HSCIF_HSSCR, reg);

    /* Set the pointer to a function that outputs one character. */
    rcar_putc = hscif_console_putc;
}
/* End of function scif_console_init(void) */

void scif_init(void)
{
    scif_module_start();
    scif_pfc_init();
    scif_console_init();
}
/* End of function scif_init(void) */

void console_putc(uint8_t outchar)
{
    rcar_putc(outchar);
}
/* End of function console_putc(void) */


static void scif_console_putc(uint8_t outchar)
{
    uint16_t reg;

    /* Check that transfer of SCIF0 is completed */
    while (!((TRANS_END_CHECK & mem_read16(SCIF_SCFSR)) == TRANS_END_CHECK))
    {
        ;
    }

    mem_write8(SCIF_SCFTDR, outchar);   /* Transfer one character */
    reg = mem_read16(SCIF_SCFSR);
    reg &= (uint16_t)(~(TRANS_END_CHECK)); /* TEND,TDFE clear */
    mem_write16(SCIF_SCFSR, reg);

    /* Check that transfer of SCIF0 is completed */
    while (!((TRANS_END_CHECK & mem_read16(SCIF_SCFSR)) == TRANS_END_CHECK))
    {
        ;
    }
}
/* End of function scif_console_putc(uint8_t outchar) */

void hscif_console_putc(uint8_t outchar)
{
    uint16_t reg;

    /* Check that transfer of SCIF0 is completed */
    while (!((TRANS_END_CHECK & mem_read16(HSCIF_HSFSR)) == TRANS_END_CHECK))
    {
        ;
    }

    mem_write8(HSCIF_HSFTDR, outchar);  /* Transfer one character */
    reg = mem_read16(HSCIF_HSFSR);
    reg &= (uint16_t)(~(TRANS_END_CHECK)); /* TEND,TDFE clear */
    mem_write16(HSCIF_HSFSR, reg);

    /* Check that transfer of SCIF0 is completed */
    while (!((TRANS_END_CHECK & mem_read16(HSCIF_HSFSR)) == TRANS_END_CHECK))
    {
        ;
    }
}
/* End of function hscif_console_putc(uint8_t outchar) */