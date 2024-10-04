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
 * DESCRIPTION   : Memory access driver header
 ******************************************************************************/
#ifndef MEM_IO_H_
#define MEM_IO_H_

#include <stdint.h>

static inline void mem_write8(uintptr_t addr, uint8_t data)
{
    *(volatile uint8_t*)addr = data;
}

static inline uint8_t mem_read8(uintptr_t addr)
{
    return (*(volatile uint8_t*)addr);
}

static inline void mem_write16(uintptr_t addr, uint16_t data)
{
    *(volatile uint16_t*)addr = data;
}

static inline uint16_t mem_read16(uintptr_t addr)
{
    return (*(volatile uint16_t*)addr);
}

static inline void mem_write32(uintptr_t addr, uint32_t data)
{
    *(volatile uint32_t*)addr = data;
}

static inline uint32_t mem_read32(uintptr_t addr)
{
    return (*(volatile uint32_t*)addr);
}

static inline void mem_write64(uintptr_t addr, uint64_t data)
{
    *(volatile uint64_t*)addr = data;
}

static inline uint64_t mem_read64(uintptr_t addr)
{
    return (*(volatile uint64_t*)addr);
}

static inline void mem_bitclrset32(uintptr_t addr, uint32_t clr, uint32_t set)
{
    mem_write32(addr, (mem_read32(addr) & ~clr) | set);
}

#if defined(__RH850G3K__)
#define mmio_write_32(a,b)          mem_write32(a,b)
#define mmio_read_32(a)             mem_read32(a)
#define mmio_clrsetbits_32(a,b,c)   mem_bitclrset32(a,b,c)
#endif


#endif /* MEM_IO_H_ */
