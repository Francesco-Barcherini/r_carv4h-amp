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
 * DESCRIPTION   : CPG register header
 ******************************************************************************/

#ifndef CPG_REGISTER_H__
#define CPG_REGISTER_H__

//#include <remap_register.h>

/* CPG base address */
/* 0xE6150000 */
#define CPG_BASE    0xE6150000//(BASE_CPG_ADDR)

/* Domain Offset Addresses */
#define D0      (0x4000U * 0U)
#define D1      (0x4000U * 1U)
#define D2      (0x4000U * 2U)
#define D3      (0x4000U * 3U)

/* CPG write protect */
#define CPG_CPGWPR  (CPG_BASE + 0x0000U + D0)
/* CPG write protect control */
#define CPG_CPGWPCR (CPG_BASE + 0x0004U + D0)

/* Module Stop Control register */
/* Registers of domain 0 */
#define CPG_MSTPCR5D0   (CPG_BASE + 0x2D14U + D0)	/* HSCIF0 */
#define CPG_MSTPCR7D0   (CPG_BASE + 0x2D1CU + D0)	/* SCIF0  */
#define CPG_MSTPCR28D0  (CPG_BASE + 0x2D70U + D0)	/* FCPR   */

/* Module Stop Status register */
/* Registers of domain 0 */
#define CPG_MSTPSR5D0   (CPG_BASE + 0x2E14U + D0)	/* HSCIF0 */
#define CPG_MSTPSR7D0   (CPG_BASE + 0x2E1CU + D0)	/* SCIF0  */
#define CPG_MSTPSR28D0  (CPG_BASE + 0x2E70U + D0)	/* FCPR   */

/* APMU */
// #define APMU_BASE           (BASE_APMU_ADDR)
// #if (RCAR_LSI == RCAR_S4)
// #define CORTEX_R_CORE       (0U)                                              /* Target Cortex R52 cores (0 only) */
// #elif ((RCAR_LSI == RCAR_V4H) || (RCAR_LSI == RCAR_V4M))
// #define CORTEX_R_CORE       (1U)                                              /* Target Cortex R52 cores (0 to 2) */
// #endif /* RCAR_LSI == RCAR_S4 */
// #define APMU_WPR   			(APMU_BASE + 0x0004U)	/* Write protection register */
// #define APMU_D0_ACCENR  	(APMU_BASE + 0x0010U)	/* Domain0 Access enable register */
// #define APMU_D1_ACCENR  	(APMU_BASE + 0x0014U)	/* Domain1 Access enable register */
// #define APMU_D2_ACCENR  	(APMU_BASE + 0x0018U)	/* Domain2 Access enable register */
// #define APMU_D3_ACCENR		(APMU_BASE + 0x001CU)	/* Domain3 Access enable register */
// #define APMU_CRRSTCTRL      (APMU_BASE + (CORTEX_R_CORE * 0x40U) + 0x0304U)   /* Cortex-R Reset Control Register    */
// #define APMU_CRBAR          (APMU_BASE + (CORTEX_R_CORE * 0x40U) + 0x0334U)   /* Cortex-R Boot Address Register */
// #define APMU_CRBARP         (APMU_BASE + (CORTEX_R_CORE * 0x40U) + 0x033CU)   /* Cortex-R Boot Address Register Protected   */
// #define APMU_PWRCTRLC0      (APMU_BASE + 0x0800U)                             /* Power Control Register for Core 0 */
// #define APMU_RVBARPLC0      (APMU_BASE + 0x0838U)                             /* Reset Vector Base Address Register Protected Low  for Core 0 */
// #define APMU_RVBARPHC0      (APMU_BASE + 0x083CU)                             /* Reset Vector Base Address Register Protected High for Core 0 */

/* SD-IF */
#define CPG_SD0CKCR0    (CPG_BASE + 0x0870U)

/* RPC */
#define CPG_RPCCKCR    (CPG_BASE + 0x0874U)

#endif  /* CPG_REGISTER_H__ */
