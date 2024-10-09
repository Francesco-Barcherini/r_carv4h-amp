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
 * DESCRIPTION   : RST register header
 ******************************************************************************/

#ifndef RST_REGISTER_H_
#define RST_REGISTER_H_

//#include <remap_register.h>

#define RST_BASE        (BASE_RESET_ADDR)   /* 0xE6160000 */

#define RST_MODEMR0         (RST_BASE + 0x0000U)    /* Mode pin register0 */
#define RST_MODEMR1         (RST_BASE + 0x0004U)    /* Mode pin register1 */
#define RST_MODEMR0_MD31    (1U << 31U)
#define RST_MODEMR1_MD32    (1U << 0U)

#define RST_MODEMR0_BOOT_DEV_MASK           (0x0000001EU)
#define RST_MODEMR0_BOOT_DEV_HYPERFLASH160  (0x00000004U)
#define RST_MODEMR0_BOOT_DEV_HYPERFLASH80   (0x00000006U)
#define RST_MODEMR0_BOOT_DEV_QSPI_FLASH40   (0x00000008U)
#define RST_MODEMR0_BOOT_DEV_EMMC_50X8      (0x0000001AU)
#define RST_WDTRSTCR        (RST_BASE + 0x0010U)

/* SCIF / HSCIF clock speed */
#define MODEMR_SCIF_DLMODE          (0x00000000U)
#define MODEMR_HSCIF_DLMODE_921600  (0x00000001U)
#define MODEMR_HSCIF_DLMODE_1843200 (0x00000002U)
#define MODEMR_HSCIF_DLMODE_3000000 (0x00000003U)

#endif /* RST_REGISTER_H_ */
