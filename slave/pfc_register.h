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
 * DESCRIPTION   : PFC register header
 ******************************************************************************/


#ifndef PFC_REGISTER_H__
#define PFC_REGISTER_H__

#include <stdint.h>
//#include <remap_register.h>

/* PFC / GPIO base address */
/* 0xE6050000 */
#define PFC_BASE        (0xE6050000)

#define PFC_RW_OFFSET   (0x0000U)
#define PFC_SET_OFFSET  (0x0200U)
#define PFC_CLR_OFFSET  (0x0400U)

#if (RCAR_LSI == RCAR_S4)
#define PFC_MCU_BASE    (BASE_PFCMCU_ADDR)

#define PFC_PORT_GRP0   (0x00000000U)   /* Port Group0/4 */
#define PFC_PORT_GRP1   (0x00000800U)   /* Port Group1/5 */
#define PFC_PORT_GRP2   (0x00001000U)   /* Port Group2/6 */
#define PFC_PORT_GRP3   (0x00001800U)   /* Port Group3/7 */
#define PFC_SYS_GRP     (0x00028000U)   /* System Group0/1 */
#elif ((RCAR_LSI == RCAR_V4H) || (RCAR_LSI == RCAR_V4M))
#define PFC_PORT_GRP0   (0x00000000U)   /* Port Group0 */
#define PFC_PORT_GRP1   (0x00000800U)   /* Port Group1 */
#define PFC_PORT_GRP2   (0x00008000U)   /* Port Group2 */
#define PFC_PORT_GRP3   (0x00008800U)   /* Port Group3 */
#define PFC_PORT_GRP4   (0x00010000U)   /* Port Group4 */
#define PFC_PORT_GRP5   (0x00010800U)   /* Port Group5 */
#define PFC_PORT_GRP6   (0x00011000U)   /* Port Group6 */
#define PFC_PORT_GRP7   (0x00011800U)   /* Port Group7 */
#define PFC_PORT_GRP8   (0x00018000U)   /* Port Group8 */
#define PFC_SYS_GRP     (0x00028000U)   /* System Group0 */
#endif

/* PFC / GPIO registers */
/* Read/Write registers */
#define PFC_PORT_GRP_MASK   (0xFFFFF800U)

/* Port Group0 */
#define PFC_GPSR0_RW        (PFC_BASE + PFC_PORT_GRP0 + PFC_RW_OFFSET + 0x0040U)
#define PFC_IP0SR0_RW       (PFC_BASE + PFC_PORT_GRP0 + PFC_RW_OFFSET + 0x0060U)
#define PFC_POC0_RW         (PFC_BASE + PFC_PORT_GRP0 + PFC_RW_OFFSET + 0x00A0U)
/* Port Group1 */
#define PFC_GPSR1_RW        (PFC_BASE + PFC_PORT_GRP1 + PFC_RW_OFFSET + 0x0040U)
#define PFC_IP1SR1_RW       (PFC_BASE + PFC_PORT_GRP1 + PFC_RW_OFFSET + 0x0064U)
#define PFC_IP2SR1_RW       (PFC_BASE + PFC_PORT_GRP1 + PFC_RW_OFFSET + 0x0068U)
#define PFC_POC1_RW         (PFC_BASE + PFC_PORT_GRP1 + PFC_RW_OFFSET + 0x00A0U)
/* Port Group2 */
#define PFC_PUEN2_RW        (PFC_BASE + PFC_PORT_GRP2 + PFC_RW_OFFSET + 0x00C0U)
/* Port Group3 */
#define PFC_POC3_RW         (PFC_BASE + PFC_PORT_GRP3 + PFC_RW_OFFSET + 0x00A0U)


static inline uint32_t get_pmmr_addr(uint32_t addr)
{
    return (addr & PFC_PORT_GRP_MASK);
}

#endif  /* PFC_REGISTER_H__ */
