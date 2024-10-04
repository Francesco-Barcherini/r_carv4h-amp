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
 * DESCRIPTION   : CPG driver header
 ******************************************************************************/
#ifndef CPG_H_
#define CPG_H_

#include <stdint.h>
#include <mem_io.h>
#include <cpg_register.h>

#define RPC_CLK_160MHZ (0x00000013U)    /* RPCphi = 160MHz RPCD2phi = 80MHZ */

/* Prototype */
void cpg_init(void);

/* Inline function */
static inline void cpg_reg_write(uint32_t mstpcr, uint32_t mstpsr, uint32_t data)
{
    mem_write32(CPG_CPGWPR, ~data);
    mem_write32(mstpcr, data);
    while ((mem_read32(mstpsr) & ~(uint32_t)(data)) != 0U)
    {
        /* Loop to wait for confirmation that changes to "MSTPCRn" are reflected in "MSTPSRn". */
    }
}

/* Inline function */
static inline void apmu_reg_write(uint32_t reg, uint32_t data)
{
    //mem_write32(APMU_WPR, ~data);
    mem_write32(reg, data);
}

#endif /* CPG_H_ */
