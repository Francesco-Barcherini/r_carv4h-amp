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
 * Copyright 2021-2022 Renesas Electronics Corporation All rights reserved.
 *******************************************************************************/

/*******************************************************************************
 * DESCRIPTION   : HSCIF register header
 ******************************************************************************/


#ifndef HSCIF_REGISTER_H_
#define HSCIF_REGISTER_H_

//#include <remap_register.h>

/* HSCIF0 base address */
/* 0xE6540000U */
#define HSCIF0_BASE     (0xE6540000U)

#define HSCIF_HSSMR     (HSCIF0_BASE + 0x0000U) /*  16  Serial mode register */
#define HSCIF_HSBRR     (HSCIF0_BASE + 0x0004U) /*  8   Bit rate register */
#define HSCIF_HSSCR     (HSCIF0_BASE + 0x0008U) /*  16  Serial control register */
#define HSCIF_HSFTDR    (HSCIF0_BASE + 0x000CU) /*  8   Transmit FIFO data register */
#define HSCIF_HSFSR     (HSCIF0_BASE + 0x0010U) /*  16  Serial status register */
#define HSCIF_HSFCR     (HSCIF0_BASE + 0x0018U) /*  16  FIFO control register */
#define HSCIF_HSLSR     (HSCIF0_BASE + 0x0024U) /*  16  Line status register */
#define HSCIF_DL        (HSCIF0_BASE + 0x0030U) /*  16  Frequency division register */
#define HSCIF_CKS       (HSCIF0_BASE + 0x0034U) /*  16  Clock Select register */
#define HSCIF_HSSRR     (HSCIF0_BASE + 0x0040U) /*  16  Sampling rate register */

#endif /* HSCIF_REGISTER_H_ */
