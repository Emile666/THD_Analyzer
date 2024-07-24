/*==================================================================
  File Name: stm8s105.h
  Author   : Emile
  ------------------------------------------------------------------
  Purpose  : This files contains the register definition for the
             STM8S105 microcontroller.
  ------------------------------------------------------------------
  This file is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this software. If not, see <http://www.gnu.org/licenses/>.
  ================================================================== */
#ifndef _STH8S105_H_
#define _STH8S105_H_

//------------------------------------------------------------------------
// Set in IAR Project->Options->C/C++ compiler->Defined symbols if the
// default IAR header file should be used. If not defined, this header
// file (stm8s105.h) is used.
//------------------------------------------------------------------------
#if defined(USE_IAR_HEADER_FILE)
   #include <iostm8s105c6.h>
   #include <intrinsics.h>
#else

//------------------------------------------------------------------------
// Interrupt commands specific for SDCC, substitute for IAR intrinsics.h
//------------------------------------------------------------------------
#if defined(__ICCSTM8__)
    #include <intrinsics.h>
#elif defined(__SDCC)
    #define __enable_interrupt()   {__asm__("rim\n");}  /* enable interrupts */
    #define __disable_interrupt()  {__asm__("sim\n");}  /* disable interrupts */
    #define rim()                  {__asm__("rim\n");}  /* enable interrupts */
    #define sim()                  {__asm__("sim\n");}  /* disable interrupts */
    #define __no_operation()       {__asm__("nop\n");}  /* No Operation */
    #define __trap()               {__asm__("trap\n");} /* Trap (soft IT) */
    #define __wait_for_interrupt() {__asm__("wfi\n");}  /* Wait For Interrupt */
    #define __halt()               {__asm__("halt\n");} /* Halt */
#endif

//---------------------------------------------------------------
// Union definitions for all Special Function Registers (SFR).
// Supports both byte-wise access (e.g. PA_ODR = 0) and
// bitwise access (e.g. TIM1_CR2_MMS = 5, sets bits 456 to 101).
//---------------------------------------------------------------
typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit1    : 1;
      unsigned char bit2    : 1;
      unsigned char bit3    : 1;
      unsigned char bit4    : 1;
      unsigned char bit5    : 1;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR8;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit1    : 1;
      unsigned char bit2    : 1;
      unsigned char bit3    : 1;
      unsigned char bit45   : 2;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR111121;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit1    : 1;
      unsigned char bit2    : 1;
      unsigned char bit3    : 1;
      unsigned char bit4    : 1;
      unsigned char bit56   : 2;
      unsigned char bit7    : 1;
    } bits;
} DR1111121;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit1    : 1;
      unsigned char bit2    : 1;
      unsigned char bit3    : 1;
      unsigned char bit46   : 3;
      unsigned char bit7    : 1;
    } bits;
} DR111131;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit1    : 1;
      unsigned char bit2    : 1;
      unsigned char bit35   : 3;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR11131;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit12   : 2;
      unsigned char bit3    : 1;
      unsigned char bit4    : 1;
      unsigned char bit5    : 1;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR12;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit14   : 4;
      unsigned char bit5    : 1;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR14;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit0    : 1;
      unsigned char bit17   : 7;
    } bits;
} DR17;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit01   : 2;
      unsigned char bit2    : 1;
      unsigned char bit3    : 1;
      unsigned char bit4    : 1;
      unsigned char bit5    : 1;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR21;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit01   : 2;
      unsigned char bit2    : 1;
      unsigned char bit3    : 1;
      unsigned char bit46   : 3;
      unsigned char bit7    : 1;
    } bits;
} DR21131;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit01   : 2;
      unsigned char bit23   : 2;
      unsigned char bit45   : 2;
      unsigned char bit67   : 2;
    } bits;
} DR2222;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit01   : 2;
      unsigned char bit23   : 2;
      unsigned char bit47   : 4;
    } bits;
} DR224;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit02   : 3;
      unsigned char bit3    : 1;
      unsigned char bit46   : 3;
      unsigned char bit7    : 1;
    } bits;
} DR3131;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit02   : 3;
      unsigned char bit34   : 2;
      unsigned char bit5    : 1;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR321;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit03   : 4;
      unsigned char bit4    : 1;
      unsigned char bit5    : 1;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR41;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit03   : 4;
      unsigned char bit45   : 2;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR42;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit04   : 5;
      unsigned char bit5    : 1;
      unsigned char bit67   : 2;
    } bits;
} DR512;

typedef union {
    unsigned char byte; // bytewise access
    struct {            // bitwise access
      unsigned char bit05   : 6;
      unsigned char bit6    : 1;
      unsigned char bit7    : 1;
    } bits;
} DR61;

//--------------------------------------------------
// General purpose I/O ports (GPIO): PORT A - PORT I
//--------------------------------------------------
#define PPA_ODR     0x5000
#define PA_ODR      (*((volatile DR8*)PPA_ODR)).byte
#define PA_ODR_ODR0 (*((volatile DR8*)PPA_ODR)).bits.bit0
#define PA_ODR_ODR1 (*((volatile DR8*)PPA_ODR)).bits.bit1
#define PA_ODR_ODR2 (*((volatile DR8*)PPA_ODR)).bits.bit2
#define PA_ODR_ODR3 (*((volatile DR8*)PPA_ODR)).bits.bit3
#define PA_ODR_ODR4 (*((volatile DR8*)PPA_ODR)).bits.bit4
#define PA_ODR_ODR5 (*((volatile DR8*)PPA_ODR)).bits.bit5
#define PA_ODR_ODR6 (*((volatile DR8*)PPA_ODR)).bits.bit6
#define PA_ODR_ODR7 (*((volatile DR8*)PPA_ODR)).bits.bit7

#define PPA_IDR     0x5001
#define PA_IDR      (*((volatile DR8*)PPA_IDR)).byte
#define PA_IDR_IDR0 (*((volatile DR8*)PPA_IDR)).bits.bit0
#define PA_IDR_IDR1 (*((volatile DR8*)PPA_IDR)).bits.bit1
#define PA_IDR_IDR2 (*((volatile DR8*)PPA_IDR)).bits.bit2
#define PA_IDR_IDR3 (*((volatile DR8*)PPA_IDR)).bits.bit3
#define PA_IDR_IDR4 (*((volatile DR8*)PPA_IDR)).bits.bit4
#define PA_IDR_IDR5 (*((volatile DR8*)PPA_IDR)).bits.bit5
#define PA_IDR_IDR6 (*((volatile DR8*)PPA_IDR)).bits.bit6
#define PA_IDR_IDR7 (*((volatile DR8*)PPA_IDR)).bits.bit7

#define PPA_DDR     0x5002
#define PA_DDR      (*((volatile DR8*)PPA_DDR)).byte
#define PA_DDR_DDR0 (*((volatile DR8*)PPA_DDR)).bits.bit0
#define PA_DDR_DDR1 (*((volatile DR8*)PPA_DDR)).bits.bit1
#define PA_DDR_DDR2 (*((volatile DR8*)PPA_DDR)).bits.bit2
#define PA_DDR_DDR3 (*((volatile DR8*)PPA_DDR)).bits.bit3
#define PA_DDR_DDR4 (*((volatile DR8*)PPA_DDR)).bits.bit4
#define PA_DDR_DDR5 (*((volatile DR8*)PPA_DDR)).bits.bit5
#define PA_DDR_DDR6 (*((volatile DR8*)PPA_DDR)).bits.bit6
#define PA_DDR_DDR7 (*((volatile DR8*)PPA_DDR)).bits.bit7

#define PPA_CR1     0x5003
#define PA_CR1      (*((volatile DR8*)PPA_CR1)).byte
#define PA_CR1_C10  (*((volatile DR8*)PPA_CR1)).bits.bit0
#define PA_CR1_C11  (*((volatile DR8*)PPA_CR1)).bits.bit1
#define PA_CR1_C12  (*((volatile DR8*)PPA_CR1)).bits.bit2
#define PA_CR1_C13  (*((volatile DR8*)PPA_CR1)).bits.bit3
#define PA_CR1_C14  (*((volatile DR8*)PPA_CR1)).bits.bit4
#define PA_CR1_C15  (*((volatile DR8*)PPA_CR1)).bits.bit5
#define PA_CR1_C16  (*((volatile DR8*)PPA_CR1)).bits.bit6
#define PA_CR1_C17  (*((volatile DR8*)PPA_CR1)).bits.bit7

#define PPA_CR2     0x5004
#define PA_CR2      (*((volatile DR8*)PPA_CR2)).byte
#define PA_CR2_C20  (*((volatile DR8*)PPA_CR2)).bits.bit0
#define PA_CR2_C21  (*((volatile DR8*)PPA_CR2)).bits.bit1
#define PA_CR2_C22  (*((volatile DR8*)PPA_CR2)).bits.bit2
#define PA_CR2_C23  (*((volatile DR8*)PPA_CR2)).bits.bit3
#define PA_CR2_C24  (*((volatile DR8*)PPA_CR2)).bits.bit4
#define PA_CR2_C25  (*((volatile DR8*)PPA_CR2)).bits.bit5
#define PA_CR2_C26  (*((volatile DR8*)PPA_CR2)).bits.bit6
#define PA_CR2_C27  (*((volatile DR8*)PPA_CR2)).bits.bit7

#define PPB_ODR     0x5005
#define PB_ODR      (*((volatile DR8*)PPB_ODR)).byte
#define PB_ODR_ODR0 (*((volatile DR8*)PPB_ODR)).bits.bit0
#define PB_ODR_ODR1 (*((volatile DR8*)PPB_ODR)).bits.bit1
#define PB_ODR_ODR2 (*((volatile DR8*)PPB_ODR)).bits.bit2
#define PB_ODR_ODR3 (*((volatile DR8*)PPB_ODR)).bits.bit3
#define PB_ODR_ODR4 (*((volatile DR8*)PPB_ODR)).bits.bit4
#define PB_ODR_ODR5 (*((volatile DR8*)PPB_ODR)).bits.bit5
#define PB_ODR_ODR6 (*((volatile DR8*)PPB_ODR)).bits.bit6
#define PB_ODR_ODR7 (*((volatile DR8*)PPB_ODR)).bits.bit7

#define PPB_IDR     0x5006
#define PB_IDR      (*((volatile DR8*)PPB_IDR)).byte
#define PB_IDR_IDR0 (*((volatile DR8*)PPB_IDR)).bits.bit0
#define PB_IDR_IDR1 (*((volatile DR8*)PPB_IDR)).bits.bit1
#define PB_IDR_IDR2 (*((volatile DR8*)PPB_IDR)).bits.bit2
#define PB_IDR_IDR3 (*((volatile DR8*)PPB_IDR)).bits.bit3
#define PB_IDR_IDR4 (*((volatile DR8*)PPB_IDR)).bits.bit4
#define PB_IDR_IDR5 (*((volatile DR8*)PPB_IDR)).bits.bit5
#define PB_IDR_IDR6 (*((volatile DR8*)PPB_IDR)).bits.bit6
#define PB_IDR_IDR7 (*((volatile DR8*)PPB_IDR)).bits.bit7

#define PPB_DDR     0x5007
#define PB_DDR      (*((volatile DR8*)PPB_DDR)).byte
#define PB_DDR_DDR0 (*((volatile DR8*)PPB_DDR)).bits.bit0
#define PB_DDR_DDR1 (*((volatile DR8*)PPB_DDR)).bits.bit1
#define PB_DDR_DDR2 (*((volatile DR8*)PPB_DDR)).bits.bit2
#define PB_DDR_DDR3 (*((volatile DR8*)PPB_DDR)).bits.bit3
#define PB_DDR_DDR4 (*((volatile DR8*)PPB_DDR)).bits.bit4
#define PB_DDR_DDR5 (*((volatile DR8*)PPB_DDR)).bits.bit5
#define PB_DDR_DDR6 (*((volatile DR8*)PPB_DDR)).bits.bit6
#define PB_DDR_DDR7 (*((volatile DR8*)PPB_DDR)).bits.bit7

#define PPB_CR1     0x5008
#define PB_CR1      (*((volatile DR8*)PPB_CR1)).byte
#define PB_CR1_C10  (*((volatile DR8*)PPB_CR1)).bits.bit0
#define PB_CR1_C11  (*((volatile DR8*)PPB_CR1)).bits.bit1
#define PB_CR1_C12  (*((volatile DR8*)PPB_CR1)).bits.bit2
#define PB_CR1_C13  (*((volatile DR8*)PPB_CR1)).bits.bit3
#define PB_CR1_C14  (*((volatile DR8*)PPB_CR1)).bits.bit4
#define PB_CR1_C15  (*((volatile DR8*)PPB_CR1)).bits.bit5
#define PB_CR1_C16  (*((volatile DR8*)PPB_CR1)).bits.bit6
#define PB_CR1_C17  (*((volatile DR8*)PPB_CR1)).bits.bit7

#define PPB_CR2     0x5009
#define PB_CR2      (*((volatile DR8*)PPB_CR2)).byte
#define PB_CR2_C20  (*((volatile DR8*)PPB_CR2)).bits.bit0
#define PB_CR2_C21  (*((volatile DR8*)PPB_CR2)).bits.bit1
#define PB_CR2_C22  (*((volatile DR8*)PPB_CR2)).bits.bit2
#define PB_CR2_C23  (*((volatile DR8*)PPB_CR2)).bits.bit3
#define PB_CR2_C24  (*((volatile DR8*)PPB_CR2)).bits.bit4
#define PB_CR2_C25  (*((volatile DR8*)PPB_CR2)).bits.bit5
#define PB_CR2_C26  (*((volatile DR8*)PPB_CR2)).bits.bit6
#define PB_CR2_C27  (*((volatile DR8*)PPB_CR2)).bits.bit7

#define PPC_ODR     0x500A
#define PC_ODR      (*((volatile DR8*)PPC_ODR)).byte
#define PC_ODR_ODR0 (*((volatile DR8*)PPC_ODR)).bits.bit0
#define PC_ODR_ODR1 (*((volatile DR8*)PPC_ODR)).bits.bit1
#define PC_ODR_ODR2 (*((volatile DR8*)PPC_ODR)).bits.bit2
#define PC_ODR_ODR3 (*((volatile DR8*)PPC_ODR)).bits.bit3
#define PC_ODR_ODR4 (*((volatile DR8*)PPC_ODR)).bits.bit4
#define PC_ODR_ODR5 (*((volatile DR8*)PPC_ODR)).bits.bit5
#define PC_ODR_ODR6 (*((volatile DR8*)PPC_ODR)).bits.bit6
#define PC_ODR_ODR7 (*((volatile DR8*)PPC_ODR)).bits.bit7

#define PPC_IDR     0x500B
#define PC_IDR      (*((volatile DR8*)PPC_IDR)).byte
#define PC_IDR_IDR0 (*((volatile DR8*)PPC_IDR)).bits.bit0
#define PC_IDR_IDR1 (*((volatile DR8*)PPC_IDR)).bits.bit1
#define PC_IDR_IDR2 (*((volatile DR8*)PPC_IDR)).bits.bit2
#define PC_IDR_IDR3 (*((volatile DR8*)PPC_IDR)).bits.bit3
#define PC_IDR_IDR4 (*((volatile DR8*)PPC_IDR)).bits.bit4
#define PC_IDR_IDR5 (*((volatile DR8*)PPC_IDR)).bits.bit5
#define PC_IDR_IDR6 (*((volatile DR8*)PPC_IDR)).bits.bit6
#define PC_IDR_IDR7 (*((volatile DR8*)PPC_IDR)).bits.bit7

#define PPC_DDR     0x500C
#define PC_DDR      (*((volatile DR8*)PPC_DDR)).byte
#define PC_DDR_DDR0 (*((volatile DR8*)PPC_DDR)).bits.bit0
#define PC_DDR_DDR1 (*((volatile DR8*)PPC_DDR)).bits.bit1
#define PC_DDR_DDR2 (*((volatile DR8*)PPC_DDR)).bits.bit2
#define PC_DDR_DDR3 (*((volatile DR8*)PPC_DDR)).bits.bit3
#define PC_DDR_DDR4 (*((volatile DR8*)PPC_DDR)).bits.bit4
#define PC_DDR_DDR5 (*((volatile DR8*)PPC_DDR)).bits.bit5
#define PC_DDR_DDR6 (*((volatile DR8*)PPC_DDR)).bits.bit6
#define PC_DDR_DDR7 (*((volatile DR8*)PPC_DDR)).bits.bit7

#define PPC_CR1     0x500D
#define PC_CR1      (*((volatile DR8*)PPC_CR1)).byte
#define PC_CR1_C10  (*((volatile DR8*)PPC_CR1)).bits.bit0
#define PC_CR1_C11  (*((volatile DR8*)PPC_CR1)).bits.bit1
#define PC_CR1_C12  (*((volatile DR8*)PPC_CR1)).bits.bit2
#define PC_CR1_C13  (*((volatile DR8*)PPC_CR1)).bits.bit3
#define PC_CR1_C14  (*((volatile DR8*)PPC_CR1)).bits.bit4
#define PC_CR1_C15  (*((volatile DR8*)PPC_CR1)).bits.bit5
#define PC_CR1_C16  (*((volatile DR8*)PPC_CR1)).bits.bit6
#define PC_CR1_C17  (*((volatile DR8*)PPC_CR1)).bits.bit7

#define PPC_CR2     0x500E
#define PC_CR2      (*((volatile DR8*)PPC_CR2)).byte
#define PC_CR2_C20  (*((volatile DR8*)PPC_CR2)).bits.bit0
#define PC_CR2_C21  (*((volatile DR8*)PPC_CR2)).bits.bit1
#define PC_CR2_C22  (*((volatile DR8*)PPC_CR2)).bits.bit2
#define PC_CR2_C23  (*((volatile DR8*)PPC_CR2)).bits.bit3
#define PC_CR2_C24  (*((volatile DR8*)PPC_CR2)).bits.bit4
#define PC_CR2_C25  (*((volatile DR8*)PPC_CR2)).bits.bit5
#define PC_CR2_C26  (*((volatile DR8*)PPC_CR2)).bits.bit6
#define PC_CR2_C27  (*((volatile DR8*)PPC_CR2)).bits.bit7

#define PPD_ODR     0x500F
#define PD_ODR      (*((volatile DR8*)PPD_ODR)).byte
#define PD_ODR_ODR0 (*((volatile DR8*)PPD_ODR)).bits.bit0
#define PD_ODR_ODR1 (*((volatile DR8*)PPD_ODR)).bits.bit1
#define PD_ODR_ODR2 (*((volatile DR8*)PPD_ODR)).bits.bit2
#define PD_ODR_ODR3 (*((volatile DR8*)PPD_ODR)).bits.bit3
#define PD_ODR_ODR4 (*((volatile DR8*)PPD_ODR)).bits.bit4
#define PD_ODR_ODR5 (*((volatile DR8*)PPD_ODR)).bits.bit5
#define PD_ODR_ODR6 (*((volatile DR8*)PPD_ODR)).bits.bit6
#define PD_ODR_ODR7 (*((volatile DR8*)PPD_ODR)).bits.bit7

#define PPD_IDR     0x5010
#define PD_IDR      (*((volatile DR8*)PPD_IDR)).byte
#define PD_IDR_IDR0 (*((volatile DR8*)PPD_IDR)).bits.bit0
#define PD_IDR_IDR1 (*((volatile DR8*)PPD_IDR)).bits.bit1
#define PD_IDR_IDR2 (*((volatile DR8*)PPD_IDR)).bits.bit2
#define PD_IDR_IDR3 (*((volatile DR8*)PPD_IDR)).bits.bit3
#define PD_IDR_IDR4 (*((volatile DR8*)PPD_IDR)).bits.bit4
#define PD_IDR_IDR5 (*((volatile DR8*)PPD_IDR)).bits.bit5
#define PD_IDR_IDR6 (*((volatile DR8*)PPD_IDR)).bits.bit6
#define PD_IDR_IDR7 (*((volatile DR8*)PPD_IDR)).bits.bit7

#define PPD_DDR     0x5011
#define PD_DDR      (*((volatile DR8*)PPD_DDR)).byte
#define PD_DDR_DDR0 (*((volatile DR8*)PPD_DDR)).bits.bit0
#define PD_DDR_DDR1 (*((volatile DR8*)PPD_DDR)).bits.bit1
#define PD_DDR_DDR2 (*((volatile DR8*)PPD_DDR)).bits.bit2
#define PD_DDR_DDR3 (*((volatile DR8*)PPD_DDR)).bits.bit3
#define PD_DDR_DDR4 (*((volatile DR8*)PPD_DDR)).bits.bit4
#define PD_DDR_DDR5 (*((volatile DR8*)PPD_DDR)).bits.bit5
#define PD_DDR_DDR6 (*((volatile DR8*)PPD_DDR)).bits.bit6
#define PD_DDR_DDR7 (*((volatile DR8*)PPD_DDR)).bits.bit7

#define PPD_CR1     0x5012
#define PD_CR1      (*((volatile DR8*)PPD_CR1)).byte
#define PD_CR1_C10  (*((volatile DR8*)PPD_CR1)).bits.bit0
#define PD_CR1_C11  (*((volatile DR8*)PPD_CR1)).bits.bit1
#define PD_CR1_C12  (*((volatile DR8*)PPD_CR1)).bits.bit2
#define PD_CR1_C13  (*((volatile DR8*)PPD_CR1)).bits.bit3
#define PD_CR1_C14  (*((volatile DR8*)PPD_CR1)).bits.bit4
#define PD_CR1_C15  (*((volatile DR8*)PPD_CR1)).bits.bit5
#define PD_CR1_C16  (*((volatile DR8*)PPD_CR1)).bits.bit6
#define PD_CR1_C17  (*((volatile DR8*)PPD_CR1)).bits.bit7

#define PPD_CR2     0x5013
#define PD_CR2      (*((volatile DR8*)PPD_CR2)).byte
#define PD_CR2_C20  (*((volatile DR8*)PPD_CR2)).bits.bit0
#define PD_CR2_C21  (*((volatile DR8*)PPD_CR2)).bits.bit1
#define PD_CR2_C22  (*((volatile DR8*)PPD_CR2)).bits.bit2
#define PD_CR2_C23  (*((volatile DR8*)PPD_CR2)).bits.bit3
#define PD_CR2_C24  (*((volatile DR8*)PPD_CR2)).bits.bit4
#define PD_CR2_C25  (*((volatile DR8*)PPD_CR2)).bits.bit5
#define PD_CR2_C26  (*((volatile DR8*)PPD_CR2)).bits.bit6
#define PD_CR2_C27  (*((volatile DR8*)PPD_CR2)).bits.bit7

#define PPE_ODR     0x5014
#define PE_ODR      (*((volatile DR8*)PPE_ODR)).byte
#define PE_ODR_ODR0 (*((volatile DR8*)PPE_ODR)).bits.bit0
#define PE_ODR_ODR1 (*((volatile DR8*)PPE_ODR)).bits.bit1
#define PE_ODR_ODR2 (*((volatile DR8*)PPE_ODR)).bits.bit2
#define PE_ODR_ODR3 (*((volatile DR8*)PPE_ODR)).bits.bit3
#define PE_ODR_ODR4 (*((volatile DR8*)PPE_ODR)).bits.bit4
#define PE_ODR_ODR5 (*((volatile DR8*)PPE_ODR)).bits.bit5
#define PE_ODR_ODR6 (*((volatile DR8*)PPE_ODR)).bits.bit6
#define PE_ODR_ODR7 (*((volatile DR8*)PPE_ODR)).bits.bit7

#define PPE_IDR     0x5015
#define PE_IDR      (*((volatile DR8*)PPE_IDR)).byte
#define PE_IDR_IDR0 (*((volatile DR8*)PPE_IDR)).bits.bit0
#define PE_IDR_IDR1 (*((volatile DR8*)PPE_IDR)).bits.bit1
#define PE_IDR_IDR2 (*((volatile DR8*)PPE_IDR)).bits.bit2
#define PE_IDR_IDR3 (*((volatile DR8*)PPE_IDR)).bits.bit3
#define PE_IDR_IDR4 (*((volatile DR8*)PPE_IDR)).bits.bit4
#define PE_IDR_IDR5 (*((volatile DR8*)PPE_IDR)).bits.bit5
#define PE_IDR_IDR6 (*((volatile DR8*)PPE_IDR)).bits.bit6
#define PE_IDR_IDR7 (*((volatile DR8*)PPE_IDR)).bits.bit7

#define PPE_DDR     0x5016
#define PE_DDR      (*((volatile DR8*)PPE_DDR)).byte
#define PE_DDR_DDR0 (*((volatile DR8*)PPE_DDR)).bits.bit0
#define PE_DDR_DDR1 (*((volatile DR8*)PPE_DDR)).bits.bit1
#define PE_DDR_DDR2 (*((volatile DR8*)PPE_DDR)).bits.bit2
#define PE_DDR_DDR3 (*((volatile DR8*)PPE_DDR)).bits.bit3
#define PE_DDR_DDR4 (*((volatile DR8*)PPE_DDR)).bits.bit4
#define PE_DDR_DDR5 (*((volatile DR8*)PPE_DDR)).bits.bit5
#define PE_DDR_DDR6 (*((volatile DR8*)PPE_DDR)).bits.bit6
#define PE_DDR_DDR7 (*((volatile DR8*)PPE_DDR)).bits.bit7

#define PPE_CR1     0x5017
#define PE_CR1      (*((volatile DR8*)PPE_CR1)).byte
#define PE_CR1_C10  (*((volatile DR8*)PPE_CR1)).bits.bit0
#define PE_CR1_C11  (*((volatile DR8*)PPE_CR1)).bits.bit1
#define PE_CR1_C12  (*((volatile DR8*)PPE_CR1)).bits.bit2
#define PE_CR1_C13  (*((volatile DR8*)PPE_CR1)).bits.bit3
#define PE_CR1_C14  (*((volatile DR8*)PPE_CR1)).bits.bit4
#define PE_CR1_C15  (*((volatile DR8*)PPE_CR1)).bits.bit5
#define PE_CR1_C16  (*((volatile DR8*)PPE_CR1)).bits.bit6
#define PE_CR1_C17  (*((volatile DR8*)PPE_CR1)).bits.bit7

#define PPE_CR2     0x5018
#define PE_CR2      (*((volatile DR8*)PPE_CR2)).byte
#define PE_CR2_C20  (*((volatile DR8*)PPE_CR2)).bits.bit0
#define PE_CR2_C21  (*((volatile DR8*)PPE_CR2)).bits.bit1
#define PE_CR2_C22  (*((volatile DR8*)PPE_CR2)).bits.bit2
#define PE_CR2_C23  (*((volatile DR8*)PPE_CR2)).bits.bit3
#define PE_CR2_C24  (*((volatile DR8*)PPE_CR2)).bits.bit4
#define PE_CR2_C25  (*((volatile DR8*)PPE_CR2)).bits.bit5
#define PE_CR2_C26  (*((volatile DR8*)PPE_CR2)).bits.bit6
#define PE_CR2_C27  (*((volatile DR8*)PPE_CR2)).bits.bit7

#define PPF_ODR     0x5019
#define PF_ODR      (*((volatile DR8*)PPF_ODR)).byte
#define PF_ODR_ODR0 (*((volatile DR8*)PPF_ODR)).bits.bit0
#define PF_ODR_ODR1 (*((volatile DR8*)PPF_ODR)).bits.bit1
#define PF_ODR_ODR2 (*((volatile DR8*)PPF_ODR)).bits.bit2
#define PF_ODR_ODR3 (*((volatile DR8*)PPF_ODR)).bits.bit3
#define PF_ODR_ODR4 (*((volatile DR8*)PPF_ODR)).bits.bit4
#define PF_ODR_ODR5 (*((volatile DR8*)PPF_ODR)).bits.bit5
#define PF_ODR_ODR6 (*((volatile DR8*)PPF_ODR)).bits.bit6
#define PF_ODR_ODR7 (*((volatile DR8*)PPF_ODR)).bits.bit7

#define PPF_IDR     0x501A
#define PF_IDR      (*((volatile DR8*)PPF_IDR)).byte
#define PF_IDR_IDR0 (*((volatile DR8*)PPF_IDR)).bits.bit0
#define PF_IDR_IDR1 (*((volatile DR8*)PPF_IDR)).bits.bit1
#define PF_IDR_IDR2 (*((volatile DR8*)PPF_IDR)).bits.bit2
#define PF_IDR_IDR3 (*((volatile DR8*)PPF_IDR)).bits.bit3
#define PF_IDR_IDR4 (*((volatile DR8*)PPF_IDR)).bits.bit4
#define PF_IDR_IDR5 (*((volatile DR8*)PPF_IDR)).bits.bit5
#define PF_IDR_IDR6 (*((volatile DR8*)PPF_IDR)).bits.bit6
#define PF_IDR_IDR7 (*((volatile DR8*)PPF_IDR)).bits.bit7

#define PPF_DDR     0x501B
#define PF_DDR      (*((volatile DR8*)PPF_DDR)).byte
#define PF_DDR_DDR0 (*((volatile DR8*)PPF_DDR)).bits.bit0
#define PF_DDR_DDR1 (*((volatile DR8*)PPF_DDR)).bits.bit1
#define PF_DDR_DDR2 (*((volatile DR8*)PPF_DDR)).bits.bit2
#define PF_DDR_DDR3 (*((volatile DR8*)PPF_DDR)).bits.bit3
#define PF_DDR_DDR4 (*((volatile DR8*)PPF_DDR)).bits.bit4
#define PF_DDR_DDR5 (*((volatile DR8*)PPF_DDR)).bits.bit5
#define PF_DDR_DDR6 (*((volatile DR8*)PPF_DDR)).bits.bit6
#define PF_DDR_DDR7 (*((volatile DR8*)PPF_DDR)).bits.bit7

#define PPF_CR1     0x501C
#define PF_CR1      (*((volatile DR8*)PPF_CR1)).byte
#define PF_CR1_C10  (*((volatile DR8*)PPF_CR1)).bits.bit0
#define PF_CR1_C11  (*((volatile DR8*)PPF_CR1)).bits.bit1
#define PF_CR1_C12  (*((volatile DR8*)PPF_CR1)).bits.bit2
#define PF_CR1_C13  (*((volatile DR8*)PPF_CR1)).bits.bit3
#define PF_CR1_C14  (*((volatile DR8*)PPF_CR1)).bits.bit4
#define PF_CR1_C15  (*((volatile DR8*)PPF_CR1)).bits.bit5
#define PF_CR1_C16  (*((volatile DR8*)PPF_CR1)).bits.bit6
#define PF_CR1_C17  (*((volatile DR8*)PPF_CR1)).bits.bit7

#define PPF_CR2     0x501D
#define PF_CR2      (*((volatile DR8*)PPF_CR2)).byte
#define PF_CR2_C20  (*((volatile DR8*)PPF_CR2)).bits.bit0
#define PF_CR2_C21  (*((volatile DR8*)PPF_CR2)).bits.bit1
#define PF_CR2_C22  (*((volatile DR8*)PPF_CR2)).bits.bit2
#define PF_CR2_C23  (*((volatile DR8*)PPF_CR2)).bits.bit3
#define PF_CR2_C24  (*((volatile DR8*)PPF_CR2)).bits.bit4
#define PF_CR2_C25  (*((volatile DR8*)PPF_CR2)).bits.bit5
#define PF_CR2_C26  (*((volatile DR8*)PPF_CR2)).bits.bit6
#define PF_CR2_C27  (*((volatile DR8*)PPF_CR2)).bits.bit7

#define PPG_ODR     0x501E
#define PG_ODR      (*((volatile DR8*)PPG_ODR)).byte
#define PG_ODR_ODR0 (*((volatile DR8*)PPG_ODR)).bits.bit0
#define PG_ODR_ODR1 (*((volatile DR8*)PPG_ODR)).bits.bit1
#define PG_ODR_ODR2 (*((volatile DR8*)PPG_ODR)).bits.bit2
#define PG_ODR_ODR3 (*((volatile DR8*)PPG_ODR)).bits.bit3
#define PG_ODR_ODR4 (*((volatile DR8*)PPG_ODR)).bits.bit4
#define PG_ODR_ODR5 (*((volatile DR8*)PPG_ODR)).bits.bit5
#define PG_ODR_ODR6 (*((volatile DR8*)PPG_ODR)).bits.bit6
#define PG_ODR_ODR7 (*((volatile DR8*)PPG_ODR)).bits.bit7

#define PPG_IDR     0x501F
#define PG_IDR      (*((volatile DR8*)PPG_IDR)).byte
#define PG_IDR_IDR0 (*((volatile DR8*)PPG_IDR)).bits.bit0
#define PG_IDR_IDR1 (*((volatile DR8*)PPG_IDR)).bits.bit1
#define PG_IDR_IDR2 (*((volatile DR8*)PPG_IDR)).bits.bit2
#define PG_IDR_IDR3 (*((volatile DR8*)PPG_IDR)).bits.bit3
#define PG_IDR_IDR4 (*((volatile DR8*)PPG_IDR)).bits.bit4
#define PG_IDR_IDR5 (*((volatile DR8*)PPG_IDR)).bits.bit5
#define PG_IDR_IDR6 (*((volatile DR8*)PPG_IDR)).bits.bit6
#define PG_IDR_IDR7 (*((volatile DR8*)PPG_IDR)).bits.bit7

#define PPG_DDR     0x5020
#define PG_DDR      (*((volatile DR8*)PPG_DDR)).byte
#define PG_DDR_DDR0 (*((volatile DR8*)PPG_DDR)).bits.bit0
#define PG_DDR_DDR1 (*((volatile DR8*)PPG_DDR)).bits.bit1
#define PG_DDR_DDR2 (*((volatile DR8*)PPG_DDR)).bits.bit2
#define PG_DDR_DDR3 (*((volatile DR8*)PPG_DDR)).bits.bit3
#define PG_DDR_DDR4 (*((volatile DR8*)PPG_DDR)).bits.bit4
#define PG_DDR_DDR5 (*((volatile DR8*)PPG_DDR)).bits.bit5
#define PG_DDR_DDR6 (*((volatile DR8*)PPG_DDR)).bits.bit6
#define PG_DDR_DDR7 (*((volatile DR8*)PPG_DDR)).bits.bit7

#define PPG_CR1     0x5021
#define PG_CR1      (*((volatile DR8*)PPG_CR1)).byte
#define PG_CR1_C10  (*((volatile DR8*)PPG_CR1)).bits.bit0
#define PG_CR1_C11  (*((volatile DR8*)PPG_CR1)).bits.bit1
#define PG_CR1_C12  (*((volatile DR8*)PPG_CR1)).bits.bit2
#define PG_CR1_C13  (*((volatile DR8*)PPG_CR1)).bits.bit3
#define PG_CR1_C14  (*((volatile DR8*)PPG_CR1)).bits.bit4
#define PG_CR1_C15  (*((volatile DR8*)PPG_CR1)).bits.bit5
#define PG_CR1_C16  (*((volatile DR8*)PPG_CR1)).bits.bit6
#define PG_CR1_C17  (*((volatile DR8*)PPG_CR1)).bits.bit7

#define PPG_CR2     0x5022
#define PG_CR2      (*((volatile DR8*)PPG_CR2)).byte
#define PG_CR2_C20  (*((volatile DR8*)PPG_CR2)).bits.bit0
#define PG_CR2_C21  (*((volatile DR8*)PPG_CR2)).bits.bit1
#define PG_CR2_C22  (*((volatile DR8*)PPG_CR2)).bits.bit2
#define PG_CR2_C23  (*((volatile DR8*)PPG_CR2)).bits.bit3
#define PG_CR2_C24  (*((volatile DR8*)PPG_CR2)).bits.bit4
#define PG_CR2_C25  (*((volatile DR8*)PPG_CR2)).bits.bit5
#define PG_CR2_C26  (*((volatile DR8*)PPG_CR2)).bits.bit6
#define PG_CR2_C27  (*((volatile DR8*)PPG_CR2)).bits.bit7

#define PPH_ODR     0x5023
#define PH_ODR      (*((volatile DR8*)PPH_ODR)).byte
#define PH_ODR_ODR0 (*((volatile DR8*)PPH_ODR)).bits.bit0
#define PH_ODR_ODR1 (*((volatile DR8*)PPH_ODR)).bits.bit1
#define PH_ODR_ODR2 (*((volatile DR8*)PPH_ODR)).bits.bit2
#define PH_ODR_ODR3 (*((volatile DR8*)PPH_ODR)).bits.bit3
#define PH_ODR_ODR4 (*((volatile DR8*)PPH_ODR)).bits.bit4
#define PH_ODR_ODR5 (*((volatile DR8*)PPH_ODR)).bits.bit5
#define PH_ODR_ODR6 (*((volatile DR8*)PPH_ODR)).bits.bit6
#define PH_ODR_ODR7 (*((volatile DR8*)PPH_ODR)).bits.bit7

#define PPH_IDR     0x5024
#define PH_IDR      (*((volatile DR8*)PPH_IDR)).byte
#define PH_IDR_IDR0 (*((volatile DR8*)PPH_IDR)).bits.bit0
#define PH_IDR_IDR1 (*((volatile DR8*)PPH_IDR)).bits.bit1
#define PH_IDR_IDR2 (*((volatile DR8*)PPH_IDR)).bits.bit2
#define PH_IDR_IDR3 (*((volatile DR8*)PPH_IDR)).bits.bit3
#define PH_IDR_IDR4 (*((volatile DR8*)PPH_IDR)).bits.bit4
#define PH_IDR_IDR5 (*((volatile DR8*)PPH_IDR)).bits.bit5
#define PH_IDR_IDR6 (*((volatile DR8*)PPH_IDR)).bits.bit6
#define PH_IDR_IDR7 (*((volatile DR8*)PPH_IDR)).bits.bit7

#define PPH_DDR     0x5025
#define PH_DDR      (*((volatile DR8*)PPH_DDR)).byte
#define PH_DDR_DDR0 (*((volatile DR8*)PPH_DDR)).bits.bit0
#define PH_DDR_DDR1 (*((volatile DR8*)PPH_DDR)).bits.bit1
#define PH_DDR_DDR2 (*((volatile DR8*)PPH_DDR)).bits.bit2
#define PH_DDR_DDR3 (*((volatile DR8*)PPH_DDR)).bits.bit3
#define PH_DDR_DDR4 (*((volatile DR8*)PPH_DDR)).bits.bit4
#define PH_DDR_DDR5 (*((volatile DR8*)PPH_DDR)).bits.bit5
#define PH_DDR_DDR6 (*((volatile DR8*)PPH_DDR)).bits.bit6
#define PH_DDR_DDR7 (*((volatile DR8*)PPH_DDR)).bits.bit7

#define PPH_CR1     0x5026
#define PH_CR1      (*((volatile DR8*)PPH_CR1)).byte
#define PH_CR1_C10  (*((volatile DR8*)PPH_CR1)).bits.bit0
#define PH_CR1_C11  (*((volatile DR8*)PPH_CR1)).bits.bit1
#define PH_CR1_C12  (*((volatile DR8*)PPH_CR1)).bits.bit2
#define PH_CR1_C13  (*((volatile DR8*)PPH_CR1)).bits.bit3
#define PH_CR1_C14  (*((volatile DR8*)PPH_CR1)).bits.bit4
#define PH_CR1_C15  (*((volatile DR8*)PPH_CR1)).bits.bit5
#define PH_CR1_C16  (*((volatile DR8*)PPH_CR1)).bits.bit6
#define PH_CR1_C17  (*((volatile DR8*)PPH_CR1)).bits.bit7

#define PPH_CR2     0x5027
#define PH_CR2      (*((volatile DR8*)PPH_CR2)).byte
#define PH_CR2_C20  (*((volatile DR8*)PPH_CR2)).bits.bit0
#define PH_CR2_C21  (*((volatile DR8*)PPH_CR2)).bits.bit1
#define PH_CR2_C22  (*((volatile DR8*)PPH_CR2)).bits.bit2
#define PH_CR2_C23  (*((volatile DR8*)PPH_CR2)).bits.bit3
#define PH_CR2_C24  (*((volatile DR8*)PPH_CR2)).bits.bit4
#define PH_CR2_C25  (*((volatile DR8*)PPH_CR2)).bits.bit5
#define PH_CR2_C26  (*((volatile DR8*)PPH_CR2)).bits.bit6
#define PH_CR2_C27  (*((volatile DR8*)PPH_CR2)).bits.bit7

#define PPI_ODR     0x5028
#define PI_ODR      (*((volatile DR8*)PPI_ODR)).byte
#define PI_ODR_ODR0 (*((volatile DR8*)PPI_ODR)).bits.bit0
#define PI_ODR_ODR1 (*((volatile DR8*)PPI_ODR)).bits.bit1
#define PI_ODR_ODR2 (*((volatile DR8*)PPI_ODR)).bits.bit2
#define PI_ODR_ODR3 (*((volatile DR8*)PPI_ODR)).bits.bit3
#define PI_ODR_ODR4 (*((volatile DR8*)PPI_ODR)).bits.bit4
#define PI_ODR_ODR5 (*((volatile DR8*)PPI_ODR)).bits.bit5
#define PI_ODR_ODR6 (*((volatile DR8*)PPI_ODR)).bits.bit6
#define PI_ODR_ODR7 (*((volatile DR8*)PPI_ODR)).bits.bit7

#define PPI_IDR     0x5029
#define PI_IDR      (*((volatile DR8*)PPI_IDR)).byte
#define PI_IDR_IDR0 (*((volatile DR8*)PPI_IDR)).bits.bit0
#define PI_IDR_IDR1 (*((volatile DR8*)PPI_IDR)).bits.bit1
#define PI_IDR_IDR2 (*((volatile DR8*)PPI_IDR)).bits.bit2
#define PI_IDR_IDR3 (*((volatile DR8*)PPI_IDR)).bits.bit3
#define PI_IDR_IDR4 (*((volatile DR8*)PPI_IDR)).bits.bit4
#define PI_IDR_IDR5 (*((volatile DR8*)PPI_IDR)).bits.bit5
#define PI_IDR_IDR6 (*((volatile DR8*)PPI_IDR)).bits.bit6
#define PI_IDR_IDR7 (*((volatile DR8*)PPI_IDR)).bits.bit7

#define PPI_DDR     0x502A
#define PI_DDR      (*((volatile DR8*)PPI_DDR)).byte
#define PI_DDR_DDR0 (*((volatile DR8*)PPI_DDR)).bits.bit0
#define PI_DDR_DDR1 (*((volatile DR8*)PPI_DDR)).bits.bit1
#define PI_DDR_DDR2 (*((volatile DR8*)PPI_DDR)).bits.bit2
#define PI_DDR_DDR3 (*((volatile DR8*)PPI_DDR)).bits.bit3
#define PI_DDR_DDR4 (*((volatile DR8*)PPI_DDR)).bits.bit4
#define PI_DDR_DDR5 (*((volatile DR8*)PPI_DDR)).bits.bit5
#define PI_DDR_DDR6 (*((volatile DR8*)PPI_DDR)).bits.bit6
#define PI_DDR_DDR7 (*((volatile DR8*)PPI_DDR)).bits.bit7

#define PPI_CR1     0x502B
#define PI_CR1      (*((volatile DR8*)PPI_CR1)).byte
#define PI_CR1_C10  (*((volatile DR8*)PPI_CR1)).bits.bit0
#define PI_CR1_C11  (*((volatile DR8*)PPI_CR1)).bits.bit1
#define PI_CR1_C12  (*((volatile DR8*)PPI_CR1)).bits.bit2
#define PI_CR1_C13  (*((volatile DR8*)PPI_CR1)).bits.bit3
#define PI_CR1_C14  (*((volatile DR8*)PPI_CR1)).bits.bit4
#define PI_CR1_C15  (*((volatile DR8*)PPI_CR1)).bits.bit5
#define PI_CR1_C16  (*((volatile DR8*)PPI_CR1)).bits.bit6
#define PI_CR1_C17  (*((volatile DR8*)PPI_CR1)).bits.bit7

#define PPI_CR2     0x502C
#define PI_CR2      (*((volatile DR8*)PPI_CR2)).byte
#define PI_CR2_C20  (*((volatile DR8*)PPI_CR2)).bits.bit0
#define PI_CR2_C21  (*((volatile DR8*)PPI_CR2)).bits.bit1
#define PI_CR2_C22  (*((volatile DR8*)PPI_CR2)).bits.bit2
#define PI_CR2_C23  (*((volatile DR8*)PPI_CR2)).bits.bit3
#define PI_CR2_C24  (*((volatile DR8*)PPI_CR2)).bits.bit4
#define PI_CR2_C25  (*((volatile DR8*)PPI_CR2)).bits.bit5
#define PI_CR2_C26  (*((volatile DR8*)PPI_CR2)).bits.bit6
#define PI_CR2_C27  (*((volatile DR8*)PPI_CR2)).bits.bit7

//--------------------------------------------------
// Flash program memory and data EEPROM
//--------------------------------------------------
#define PFLASH_CR1      0x505A
#define FLASH_CR1       (*((volatile DR8*)PFLASH_CR1)).byte
#define FLASH_CR1_FIX   (*((volatile DR8*)PFLASH_CR1)).bits.bit0
#define FLASH_CR1_IE    (*((volatile DR8*)PFLASH_CR1)).bits.bit1
#define FLASH_CR1_AHALT (*((volatile DR8*)PFLASH_CR1)).bits.bit2
#define FLASH_CR1_HALT  (*((volatile DR8*)PFLASH_CR1)).bits.bit3

#define PFLASH_CR2      0x505B
#define FLASH_CR2       (*((volatile DR8*)PFLASH_CR2)).byte
#define FLASH_CR2_PRG   (*((volatile DR8*)PFLASH_CR2)).bits.bit0
#define FLASH_CR2_FPRG  (*((volatile DR8*)PFLASH_CR2)).bits.bit4
#define FLASH_CR2_ERASE (*((volatile DR8*)PFLASH_CR2)).bits.bit5
#define FLASH_CR2_WPRG  (*((volatile DR8*)PFLASH_CR2)).bits.bit6
#define FLASH_CR2_OPT   (*((volatile DR8*)PFLASH_CR2)).bits.bit7

#define PFLASH_NCR2      0x505C
#define FLASH_NCR2        (*((volatile DR8*)PFLASH_NCR2)).byte
#define FLASH_NCR2_NPRG   (*((volatile DR8*)PFLASH_NCR2)).bits.bit0
#define FLASH_NCR2_NFPRG  (*((volatile DR8*)PFLASH_NCR2)).bits.bit4
#define FLASH_NCR2_NERASE (*((volatile DR8*)PFLASH_NCR2)).bits.bit5
#define FLASH_NCR2_NWPRG  (*((volatile DR8*)PFLASH_NCR2)).bits.bit6
#define FLASH_NCR2_NOPT   (*((volatile DR8*)PFLASH_NCR2)).bits.bit7

#define PFLASH_FPR      0x505D
#define FLASH_FPR       (*((volatile DR8*)PFLASH_FPR)).byte
#define FLASH_FPR_WPB0  (*((volatile DR8*)PFLASH_FPR)).bits.bit0
#define FLASH_FPR_WPB1  (*((volatile DR8*)PFLASH_FPR)).bits.bit1
#define FLASH_FPR_WPB2  (*((volatile DR8*)PFLASH_FPR)).bits.bit2
#define FLASH_FPR_WPB3  (*((volatile DR8*)PFLASH_FPR)).bits.bit3
#define FLASH_FPR_WPB4  (*((volatile DR8*)PFLASH_FPR)).bits.bit4
#define FLASH_FPR_WPB5  (*((volatile DR8*)PFLASH_FPR)).bits.bit5

#define PFLASH_NFPR      0x505E
#define FLASH_NFPR       (*((volatile DR8*)PFLASH_NFPR)).byte
#define FLASH_NFPR_WPB0  (*((volatile DR8*)PFLASH_NFPR)).bits.bit0
#define FLASH_NFPR_WPB1  (*((volatile DR8*)PFLASH_NFPR)).bits.bit1
#define FLASH_NFPR_WPB2  (*((volatile DR8*)PFLASH_NFPR)).bits.bit2
#define FLASH_NFPR_WPB3  (*((volatile DR8*)PFLASH_NFPR)).bits.bit3
#define FLASH_NFPR_WPB4  (*((volatile DR8*)PFLASH_NFPR)).bits.bit4
#define FLASH_NFPR_WPB5  (*((volatile DR8*)PFLASH_NFPR)).bits.bit5

#define PFLASH_IAPSR          0x505F
#define FLASH_IAPSR           (*((volatile DR8*)PFLASH_IAPSR)).byte
#define FLASH_IAPSR_WR_PG_DIS (*((volatile DR8*)PFLASH_IAPSR)).bits.bit0
#define FLASH_IAPSR_PUL       (*((volatile DR8*)PFLASH_IAPSR)).bits.bit1
#define FLASH_IAPSR_EOP       (*((volatile DR8*)PFLASH_IAPSR)).bits.bit2
#define FLASH_IAPSR_DUL       (*((volatile DR8*)PFLASH_IAPSR)).bits.bit3
#define FLASH_IAPSR_HVOFF     (*((volatile DR8*)PFLASH_IAPSR)).bits.bit6

#define PFLASH_PUKR           0x5062
#define FLASH_PUKR            (*((volatile DR8*)PFLASH_PUKR)).byte

#define PFLASH_DUKR           0x5064
#define FLASH_DUKR            (*((volatile DR8*)PFLASH_DUKR)).byte

//-----------------------------------------------------------
// ITC (interrupt controller) - EXTI register definitions
//-----------------------------------------------------------
#define PEXTI_CR1             0x50A0
#define EXTI_CR1              (*((volatile DR2222*)PEXTI_CR1)).byte
#define EXTI_CR1_PAIS         (*((volatile DR2222*)PEXTI_CR1)).bits.bit01
#define EXTI_CR1_PBIS         (*((volatile DR2222*)PEXTI_CR1)).bits.bit23
#define EXTI_CR1_PCIS         (*((volatile DR2222*)PEXTI_CR1)).bits.bit45
#define EXTI_CR1_PDIS         (*((volatile DR2222*)PEXTI_CR1)).bits.bit67

#define PEXTI_CR2             0x50A1
#define EXTI_CR2              (*((volatile DR21*)PEXTI_CR2)).byte
#define EXTI_CR2_PEIS         (*((volatile DR21*)PEXTI_CR2)).bits.bit01
#define EXTI_CR2_TLIS         (*((volatile DR21*)PEXTI_CR2)).bits.bit2

//--------------------------------------------------
// RST (reset) register definitions
//--------------------------------------------------
#define PRST_SR               0x50B3
#define RST_SR                (*((volatile DR8*)PRST_SR)).byte
#define RST_SR_WWDGF          (*((volatile DR8*)PRST_SR)).bits.bit0
#define RST_SR_IWDGF          (*((volatile DR8*)PRST_SR)).bits.bit1
#define RST_SR_ILLOPF         (*((volatile DR8*)PRST_SR)).bits.bit2
#define RST_SR_SWIMF          (*((volatile DR8*)PRST_SR)).bits.bit3
#define RST_SR_EMCF           (*((volatile DR8*)PRST_SR)).bits.bit4

//--------------------------------------------------
// CLK (clock control) register definitions
//--------------------------------------------------
#define PCLK_ICKR             0x50C0
#define CLK_ICKR              (*((volatile DR8*)PCLK_ICKR)).byte
#define CLK_ICKR_HSIEN        (*((volatile DR8*)PCLK_ICKR)).bits.bit0
#define CLK_ICKR_HSIRDY       (*((volatile DR8*)PCLK_ICKR)).bits.bit1
#define CLK_ICKR_FHW          (*((volatile DR8*)PCLK_ICKR)).bits.bit2
#define CLK_ICKR_LSIEN        (*((volatile DR8*)PCLK_ICKR)).bits.bit3
#define CLK_ICKR_LSIRDY       (*((volatile DR8*)PCLK_ICKR)).bits.bit4
#define CLK_ICKR_REGAH        (*((volatile DR8*)PCLK_ICKR)).bits.bit5

#define PCLK_ECKR             0x50C1
#define CLK_ECKR              (*((volatile DR8*)PCLK_ECKR)).byte
#define CLK_ECKR_HSEEN        (*((volatile DR8*)PCLK_ECKR)).bits.bit0
#define CLK_ECKR_HSERDY       (*((volatile DR8*)PCLK_ECKR)).bits.bit1

#define PCLK_CMSR             0x50C3
#define CLK_CMSR              (*((volatile DR8*)PCLK_CMSR)).byte

#define PCLK_SWR              0x50C4
#define CLK_SWR               (*((volatile DR8*)PCLK_SWR)).byte

#define PCLK_SWCR             0x50C5
#define CLK_SWCR              (*((volatile DR8*)PCLK_SWCR)).byte
#define CLK_SWCR_SWBSY        (*((volatile DR8*)PCLK_SWCR)).bits.bit0
#define CLK_SWCR_SWEN         (*((volatile DR8*)PCLK_SWCR)).bits.bit1
#define CLK_SWCR_SWIEN        (*((volatile DR8*)PCLK_SWCR)).bits.bit2
#define CLK_SWCR_SWIF         (*((volatile DR8*)PCLK_SWCR)).bits.bit3

#define PCLK_CKDIVR           0x50C6
#define CLK_CKDIVR            (*((volatile DR321*)PCLK_CKDIVR)).byte
#define CLK_CKDIVR_CPUDIV     (*((volatile DR321*)PCLK_CKDIVR)).bits.bit02
#define CLK_CKDIVR_HSIDIV     (*((volatile DR321*)PCLK_CKDIVR)).bits.bit34

#define PCLK_PCKENR1          0x50C7
#define CLK_PCKENR1           (*((volatile DR8*)PCLK_PCKENR1)).byte

#define PCLK_CSSR             0x50C8
#define CLK_CSSR              (*((volatile DR8*)PCLK_CSSR)).byte
#define CLK_CSSR_CSSEN        (*((volatile DR8*)PCLK_CSSR)).bits.bit0
#define CLK_CSSR_AUX          (*((volatile DR8*)PCLK_CSSR)).bits.bit1
#define CLK_CSSR_CSSDIE       (*((volatile DR8*)PCLK_CSSR)).bits.bit2
#define CLK_CSSR_CSSD         (*((volatile DR8*)PCLK_CSSR)).bits.bit3

#define PCLK_CCOR             0x50C9
#define CLK_CCOR              (*((volatile DR14*)PCLK_CCOR)).byte
#define CLK_CCOR_CCOEN        (*((volatile DR14*)PCLK_CCOR)).bits.bit0
#define CLK_CCOR_CCOSEL       (*((volatile DR14*)PCLK_CCOR)).bits.bit14
#define CLK_CCOR_CCORDY       (*((volatile DR14*)PCLK_CCOR)).bits.bit5
#define CLK_CCOR_CCOBSY       (*((volatile DR14*)PCLK_CCOR)).bits.bit6

#define PCLK_PCKENR2          0x50CA
#define CLK_PCKENR2           (*((volatile DR8*)PCLK_PCKENR2)).byte

#define PCLK_HSITRIMR         0x50CC
#define CLK_HSITRIMR          (*((volatile DR41*)PCLK_HSITRIMR)).byte
#define CLK_HSITRIMR_HSITRIM  (*((volatile DR41*)PCLK_HSITRIMR)).bits.bit03

#define PCLK_SWIMCCR          0x50CD
#define CLK_SWIMCCR           (*((volatile DR8*)PCLK_SWIMCCR)).byte
#define CLK_SWIMCCR_SWIMCLK   (*((volatile DR8*)PCLK_SWIMCCR)).bits.bit0

//--------------------------------------------------
// WWDG (window watchdog) register definitions
//--------------------------------------------------
#define PWWDG_CR             0x50D1
#define WWDG_CR              (*((volatile DR8*)PWWDG_CR)).byte
#define WWDG_CR_T0           (*((volatile DR8*)PWWDG_CR)).bits.bit0
#define WWDG_CR_T1           (*((volatile DR8*)PWWDG_CR)).bits.bit1
#define WWDG_CR_T2           (*((volatile DR8*)PWWDG_CR)).bits.bit2
#define WWDG_CR_T3           (*((volatile DR8*)PWWDG_CR)).bits.bit3
#define WWDG_CR_T4           (*((volatile DR8*)PWWDG_CR)).bits.bit4
#define WWDG_CR_T5           (*((volatile DR8*)PWWDG_CR)).bits.bit5
#define WWDG_CR_T6           (*((volatile DR8*)PWWDG_CR)).bits.bit6
#define WWDG_CR_WDGA         (*((volatile DR8*)PWWDG_CR)).bits.bit7

#define PWWDG_WR             0x50D2
#define WWDG_WR              (*((volatile DR8*)PWWDG_WR)).byte
#define WWDG_WR_W0           (*((volatile DR8*)PWWDG_WR)).bits.bit0
#define WWDG_WR_W1           (*((volatile DR8*)PWWDG_WR)).bits.bit1
#define WWDG_WR_W2           (*((volatile DR8*)PWWDG_WR)).bits.bit2
#define WWDG_WR_W3           (*((volatile DR8*)PWWDG_WR)).bits.bit3
#define WWDG_WR_W4           (*((volatile DR8*)PWWDG_WR)).bits.bit4
#define WWDG_WR_W5           (*((volatile DR8*)PWWDG_WR)).bits.bit5
#define WWDG_WR_W6           (*((volatile DR8*)PWWDG_WR)).bits.bit6

//--------------------------------------------------
// IWDG (independent watchdog) register definitions
//--------------------------------------------------
#define PIWDG_KR             0x50E0
#define IWDG_KR              (*((volatile DR8*)PIWDG_KR)).byte

#define PIWDG_PR             0x50E1
#define IWDG_PR              (*((volatile DR3131*)PIWDG_PR)).byte
#define IWDG_PR_PR           (*((volatile DR3131*)PIWDG_PR)).bits.bit02

#define PIWDG_RLR            0x50E2
#define IWDG_RLR             (*((volatile DR8*)PIWDG_RLR)).byte

//--------------------------------------------------
// AWU (auto-wakeup) register definitions
//--------------------------------------------------
#define PAWU_CSR             0x50F0
#define AWU_CSR              (*((volatile DR8*)PAWU_CSR)).byte
#define AWU_CSR_MSR          (*((volatile DR8*)PAWU_CSR)).bits.bit0
#define AWU_CSR_AWUEN        (*((volatile DR8*)PAWU_CSR)).bits.bit4
#define AWU_CSR_AWUF         (*((volatile DR8*)PAWU_CSR)).bits.bit5

#define PAWU_APR             0x50F1
#define AWU_APR              (*((volatile DR61*)PAWU_APR)).byte
#define AWU_APR_APR          (*((volatile DR61*)PAWU_APR)).bits.bit05

#define PAWU_TBR             0x50F2
#define AWU_TBR              (*((volatile DR41*)PAWU_TBR)).byte
#define AWU_TBR_AWUTB        (*((volatile DR41*)PAWU_TBR)).bits.bit03

//--------------------------------------------------
// BEEP (beeper) register definitions
//--------------------------------------------------
#define PBEEP_CSR            0x50F3
#define BEEP_CSR             (*((volatile DR512*)PBEEP_CSR)).byte
#define BEEP_CSR_BEEPDIV     (*((volatile DR512*)PBEEP_CSR)).bits.bit04
#define BEEP_CSR_BEEPEN      (*((volatile DR512*)PBEEP_CSR)).bits.bit5
#define BEEP_CSR_BEEPSEL     (*((volatile DR512*)PBEEP_CSR)).bits.bit67

//-----------------------------------------------------------
// SPI (Serial Peripheral Interface) register definitions
//-----------------------------------------------------------
#define PSPI_CR1             0x5200
#define SPI_CR1              (*((volatile DR11131*)PSPI_CR1)).byte
#define SPI_CR1_CPHA         (*((volatile DR11131*)PSPI_CR1)).bits.bit0
#define SPI_CR1_CPOL         (*((volatile DR11131*)PSPI_CR1)).bits.bit1
#define SPI_CR1_MSTR         (*((volatile DR11131*)PSPI_CR1)).bits.bit2
#define SPI_CR1_BR           (*((volatile DR11131*)PSPI_CR1)).bits.bit35
#define SPI_CR1_SPE          (*((volatile DR11131*)PSPI_CR1)).bits.bit6
#define SPI_CR1_LSBFIRST     (*((volatile DR11131*)PSPI_CR1)).bits.bit7

#define PSPI_CR2             0x5201
#define SPI_CR2              (*((volatile DR8*)PSPI_CR2)).byte
#define SPI_CR2_SSI          (*((volatile DR8*)PSPI_CR2)).bits.bit0
#define SPI_CR2_SSM          (*((volatile DR8*)PSPI_CR2)).bits.bit1
#define SPI_CR2_RXONLY       (*((volatile DR8*)PSPI_CR2)).bits.bit2
#define SPI_CR2_CRCNEXT      (*((volatile DR8*)PSPI_CR2)).bits.bit4
#define SPI_CR2_CECEN        (*((volatile DR8*)PSPI_CR2)).bits.bit5
#define SPI_CR2_BDOE         (*((volatile DR8*)PSPI_CR2)).bits.bit6
#define SPI_CR2_BDM          (*((volatile DR8*)PSPI_CR2)).bits.bit7

#define PSPI_ICR             0x5202
#define SPI_ICR              (*((volatile DR8*)PSPI_ICR)).byte
#define SPI_ICR_WKIE         (*((volatile DR8*)PSPI_ICR)).bits.bit4
#define SPI_ICR_ERRIE        (*((volatile DR8*)PSPI_ICR)).bits.bit5
#define SPI_ICR_RXIE         (*((volatile DR8*)PSPI_ICR)).bits.bit6
#define SPI_ICR_TXIE         (*((volatile DR8*)PSPI_ICR)).bits.bit7

#define PSPI_SR             0x5203
#define SPI_SR              (*((volatile DR8*)PSPI_SR)).byte
#define SPI_SR_RXNE         (*((volatile DR8*)PSPI_SR)).bits.bit0
#define SPI_SR_TXE          (*((volatile DR8*)PSPI_SR)).bits.bit1
#define SPI_SR_WKUP         (*((volatile DR8*)PSPI_SR)).bits.bit3
#define SPI_SR_CRCERR       (*((volatile DR8*)PSPI_SR)).bits.bit4
#define SPI_SR_MODF         (*((volatile DR8*)PSPI_SR)).bits.bit5
#define SPI_SR_OVR          (*((volatile DR8*)PSPI_SR)).bits.bit6
#define SPI_SR_BSY          (*((volatile DR8*)PSPI_SR)).bits.bit7

#define PSPI_DR             0x5204
#define SPI_DR              (*((volatile DR8*)PSPI_DR)).byte

#define PSPI_CRCPR          0x5205
#define SPI_CRCPR           (*((volatile DR8*)PSPI_CRCPR)).byte

#define PSPI_RXCRCR         0x5206
#define SPI_RXCRCR          (*((volatile DR8*)PSPI_RXCRCR)).byte

#define PSPI_TXCRCR         0x5207
#define SPI_TXCRCR          (*((volatile DR8*)PSPI_TXCRCR)).byte

//-----------------------------------------------------------
// I2C (Inter-integrated Interface) register definitions
//-----------------------------------------------------------
#define PI2C_CR1             0x5210
#define I2C_CR1              (*((volatile DR8*)PI2C_CR1)).byte
#define I2C_CR1_PE           (*((volatile DR8*)PI2C_CR1)).bits.bit0
#define I2C_CR1_ENGC         (*((volatile DR8*)PI2C_CR1)).bits.bit6
#define I2C_CR1_NOSTRETCH    (*((volatile DR8*)PI2C_CR1)).bits.bit7

#define PI2C_CR2             0x5211
#define I2C_CR2              (*((volatile DR8*)PI2C_CR2)).byte
#define I2C_CR2_START        (*((volatile DR8*)PI2C_CR2)).bits.bit0
#define I2C_CR2_STOP         (*((volatile DR8*)PI2C_CR2)).bits.bit1
#define I2C_CR2_ACK          (*((volatile DR8*)PI2C_CR2)).bits.bit2
#define I2C_CR2_POS          (*((volatile DR8*)PI2C_CR2)).bits.bit3
#define I2C_CR2_SWRST        (*((volatile DR8*)PI2C_CR2)).bits.bit7

#define PI2C_FREQR           0x5212
#define I2C_FREQR            (*((volatile DR8*)PI2C_FREQR)).byte
#define I2C_FREQR_FREQ       (*((volatile DR8*)PI2C_FREQR)).bits.bit05

#define PI2C_OARL           0x5213
#define I2C_OARL            (*((volatile DR17*)PI2C_OARL)).byte
#define I2C_OARL_ADD0       (*((volatile DR17*)PI2C_OARL)).bits.bit0
#define I2C_OARL_ADD        (*((volatile DR17*)PI2C_OARL)).bits.bit17

#define PI2C_OARH           0x5214
#define I2C_OARH            (*((volatile DR12*)PI2C_OARH)).byte
#define I2C_OARH_ADD        (*((volatile DR12*)PI2C_OARH)).bits.bit12
#define I2C_OARH_ADDCONF    (*((volatile DR12*)PI2C_OARH)).bits.bit6
#define I2C_OARH_ADDMODE    (*((volatile DR12*)PI2C_OARH)).bits.bit7

#define PI2C_DR             0x5216
#define I2C_DR              (*((volatile DR8*)PI2C_DR)).byte

#define PI2C_SR1            0x5217
#define I2C_SR1             (*((volatile DR8*)PI2C_SR1)).byte
#define I2C_SR1_SB          (*((volatile DR8*)PI2C_SR1)).bits.bit0
#define I2C_SR1_ADDR        (*((volatile DR8*)PI2C_SR1)).bits.bit1
#define I2C_SR1_BTF         (*((volatile DR8*)PI2C_SR1)).bits.bit2
#define I2C_SR1_ADD10       (*((volatile DR8*)PI2C_SR1)).bits.bit3
#define I2C_SR1_STOPF       (*((volatile DR8*)PI2C_SR1)).bits.bit4
#define I2C_SR1_RXNE        (*((volatile DR8*)PI2C_SR1)).bits.bit6
#define I2C_SR1_TXE         (*((volatile DR8*)PI2C_SR1)).bits.bit7

#define PI2C_SR2            0x5218
#define I2C_SR2             (*((volatile DR8*)PI2C_SR2)).byte
#define I2C_SR2_BERR        (*((volatile DR8*)PI2C_SR2)).bits.bit0
#define I2C_SR2_ARLO        (*((volatile DR8*)PI2C_SR2)).bits.bit1
#define I2C_SR2_AF          (*((volatile DR8*)PI2C_SR2)).bits.bit2
#define I2C_SR2_OVR         (*((volatile DR8*)PI2C_SR2)).bits.bit3
#define I2C_SR2_WUFH        (*((volatile DR8*)PI2C_SR2)).bits.bit5

#define PI2C_SR3            0x5219
#define I2C_SR3             (*((volatile DR8*)PI2C_SR3)).byte
#define I2C_SR3_MSL         (*((volatile DR8*)PI2C_SR3)).bits.bit0
#define I2C_SR3_BUSY        (*((volatile DR8*)PI2C_SR3)).bits.bit1
#define I2C_SR3_TRA         (*((volatile DR8*)PI2C_SR3)).bits.bit2
#define I2C_SR3_GENCALL     (*((volatile DR8*)PI2C_SR3)).bits.bit4
#define I2C_SR3_DUALF       (*((volatile DR8*)PI2C_SR3)).bits.bit7

#define PI2C_ITR            0x521A
#define I2C_ITR             (*((volatile DR8*)PI2C_ITR)).byte
#define I2C_ITR_ITERREN     (*((volatile DR8*)PI2C_ITR)).bits.bit0
#define I2C_ITR_ITEVTEN     (*((volatile DR8*)PI2C_ITR)).bits.bit1
#define I2C_ITR_ITBUFEN     (*((volatile DR8*)PI2C_ITR)).bits.bit2

#define PI2C_CCRL           0x521B
#define I2C_CCRL            (*((volatile DR8*)PI2C_CCRL)).byte

#define PI2C_CCRH           0x521C
#define I2C_CCRH            (*((volatile DR41*)PI2C_CCRH)).byte
#define I2C_CCRH_CCR        (*((volatile DR41*)PI2C_CCRH)).bits.bit03
#define I2C_CCRH_DUTY       (*((volatile DR41*)PI2C_CCRH)).bits.bit6
#define I2C_CCRH_F_S        (*((volatile DR41*)PI2C_CCRH)).bits.bit7

#define PI2C_TRISER         0x521D
#define I2C_TRISER          (*((volatile DR61*)PI2C_TRISER)).byte
#define I2C_TRISER_TRISE    (*((volatile DR61*)PI2C_TRISER)).bits.bit05

//------------------------------------------------------------------------
// UART (Universal asynchronous receiver transmitter) register definitions
//------------------------------------------------------------------------
#define PUART2_SR            0x5240
#define UART2_SR             (*((volatile DR8*)PUART2_SR)).byte
#define UART2_SR_PE          (*((volatile DR8*)PUART2_SR)).bits.bit0
#define UART2_SR_FE          (*((volatile DR8*)PUART2_SR)).bits.bit1
#define UART2_SR_NF          (*((volatile DR8*)PUART2_SR)).bits.bit2
#define UART2_SR_OR_LHE      (*((volatile DR8*)PUART2_SR)).bits.bit3
#define UART2_SR_IDLE        (*((volatile DR8*)PUART2_SR)).bits.bit4
#define UART2_SR_RXNE        (*((volatile DR8*)PUART2_SR)).bits.bit5
#define UART2_SR_TC          (*((volatile DR8*)PUART2_SR)).bits.bit6
#define UART2_SR_TXE         (*((volatile DR8*)PUART2_SR)).bits.bit7

#define PUART2_DR            0x5241
#define UART2_DR             (*((volatile DR8*)PUART2_DR)).byte

#define PUART2_BRR1          0x5242
#define UART2_BRR1           (*((volatile DR8*)PUART2_BRR1)).byte

#define PUART2_BRR2          0x5243
#define UART2_BRR2           (*((volatile DR8*)PUART2_BRR2)).byte

#define PUART2_CR1           0x5244
#define UART2_CR1            (*((volatile DR8*)PUART2_CR1)).byte
#define UART2_CR1_PIEN       (*((volatile DR8*)PUART2_CR1)).bits.bit0
#define UART2_CR1_PS         (*((volatile DR8*)PUART2_CR1)).bits.bit1
#define UART2_CR1_PCEN       (*((volatile DR8*)PUART2_CR1)).bits.bit2
#define UART2_CR1_WAKE       (*((volatile DR8*)PUART2_CR1)).bits.bit3
#define UART2_CR1_M          (*((volatile DR8*)PUART2_CR1)).bits.bit4
#define UART2_CR1_UARTD      (*((volatile DR8*)PUART2_CR1)).bits.bit5
#define UART2_CR1_T8         (*((volatile DR8*)PUART2_CR1)).bits.bit6
#define UART2_CR1_R8         (*((volatile DR8*)PUART2_CR1)).bits.bit7

#define PUART2_CR2           0x5245
#define UART2_CR2            (*((volatile DR8*)PUART2_CR2)).byte
#define UART2_CR2_SBK        (*((volatile DR8*)PUART2_CR2)).bits.bit0
#define UART2_CR2_RWU        (*((volatile DR8*)PUART2_CR2)).bits.bit1
#define UART2_CR2_REN        (*((volatile DR8*)PUART2_CR2)).bits.bit2
#define UART2_CR2_TEN        (*((volatile DR8*)PUART2_CR2)).bits.bit3
#define UART2_CR2_ILIEN      (*((volatile DR8*)PUART2_CR2)).bits.bit4
#define UART2_CR2_RIEN       (*((volatile DR8*)PUART2_CR2)).bits.bit5
#define UART2_CR2_TCIEN      (*((volatile DR8*)PUART2_CR2)).bits.bit6
#define UART2_CR2_TIEN       (*((volatile DR8*)PUART2_CR2)).bits.bit7

#define PUART2_CR3           0x5246
#define UART2_CR3            (*((volatile DR111121*)PUART2_CR3)).byte
#define UART2_CR3_LBCL       (*((volatile DR111121*)PUART2_CR3)).bits.bit0
#define UART2_CR3_CPHA       (*((volatile DR111121*)PUART2_CR3)).bits.bit1
#define UART2_CR3_CPOL       (*((volatile DR111121*)PUART2_CR3)).bits.bit2
#define UART2_CR3_CKEN       (*((volatile DR111121*)PUART2_CR3)).bits.bit3
#define UART2_CR3_STOP       (*((volatile DR111121*)PUART2_CR3)).bits.bit45
#define UART2_CR3_LINEN      (*((volatile DR111121*)PUART2_CR3)).bits.bit6

#define PUART2_CR4           0x5247
#define UART2_CR4            (*((volatile DR41*)PUART2_CR4)).byte
#define UART2_CR4_ADD        (*((volatile DR41*)PUART2_CR4)).bits.bit03
#define UART2_CR4_LBDF       (*((volatile DR41*)PUART2_CR4)).bits.bit4
#define UART2_CR4_LBDL       (*((volatile DR41*)PUART2_CR4)).bits.bit5
#define UART2_CR4_LBDIEN     (*((volatile DR41*)PUART2_CR4)).bits.bit6

#define PUART2_CR5           0x5248
#define UART2_CR5            (*((volatile DR8*)PUART2_CR5)).byte
#define UART2_CR5_IREN       (*((volatile DR8*)PUART2_CR5)).bits.bit1
#define UART2_CR5_IRLP       (*((volatile DR8*)PUART2_CR5)).bits.bit2
#define UART2_CR5_HDSEL      (*((volatile DR8*)PUART2_CR5)).bits.bit3
#define UART2_CR5_NACK       (*((volatile DR8*)PUART2_CR5)).bits.bit4
#define UART2_CR5_SCEN       (*((volatile DR8*)PUART2_CR5)).bits.bit5

#define PUART2_CR6           0x5249
#define UART2_CR6            (*((volatile DR8*)PUART2_CR6)).byte
#define UART2_CR6_LSF        (*((volatile DR8*)PUART2_CR6)).bits.bit0
#define UART2_CR6_LHDF       (*((volatile DR8*)PUART2_CR6)).bits.bit1
#define UART2_CR6_LHDIEN     (*((volatile DR8*)PUART2_CR6)).bits.bit2
#define UART2_CR6_LASE       (*((volatile DR8*)PUART2_CR6)).bits.bit4
#define UART2_CR6_LSLV       (*((volatile DR8*)PUART2_CR6)).bits.bit5
#define UART2_CR6_LDUM       (*((volatile DR8*)PUART2_CR6)).bits.bit7

#define PUART2_GTR           0x524A
#define UART2_GTR            (*((volatile DR8*)PUART2_GTR)).byte

#define PUART2_PSCR          0x524B
#define UART2_PSCR           (*((volatile DR8*)PUART2_PSCR)).byte

//------------------------------------------------------------------------
// TIM1 (16-bit advanced control timer) register definitions
//------------------------------------------------------------------------
#define PTIM1_CR1            0x5250
#define TIM1_CR1             (*((volatile DR1111121*)PTIM1_CR1)).byte
#define TIM1_CR1_CEN         (*((volatile DR1111121*)PTIM1_CR1)).bits.bit0
#define TIM1_CR1_UDIS        (*((volatile DR1111121*)PTIM1_CR1)).bits.bit1
#define TIM1_CR1_URS         (*((volatile DR1111121*)PTIM1_CR1)).bits.bit2
#define TIM1_CR1_OPM         (*((volatile DR1111121*)PTIM1_CR1)).bits.bit3
#define TIM1_CR1_DIR         (*((volatile DR1111121*)PTIM1_CR1)).bits.bit4
#define TIM1_CR1_CMS         (*((volatile DR1111121*)PTIM1_CR1)).bits.bit56
#define TIM1_CR1_ARPE        (*((volatile DR1111121*)PTIM1_CR1)).bits.bit7

#define PTIM1_CR2            0x5251
#define TIM1_CR2             (*((volatile DR111131*)PTIM1_CR2)).byte
#define TIM1_CR2_CCPC        (*((volatile DR111131*)PTIM1_CR2)).bits.bit0
#define TIM1_CR2_COMS        (*((volatile DR111131*)PTIM1_CR2)).bits.bit2
#define TIM1_CR2_MMS         (*((volatile DR111131*)PTIM1_CR2)).bits.bit46

#define PTIM1_SMCR           0x5252
#define TIM1_SMCR            (*((volatile DR3131*)PTIM1_SMCR)).byte
#define TIM1_SMCR_SMS        (*((volatile DR3131*)PTIM1_SMCR)).bits.bit02
#define TIM1_SMCR_TS         (*((volatile DR3131*)PTIM1_SMCR)).bits.bit46
#define TIM1_SMCR_MSM        (*((volatile DR3131*)PTIM1_SMCR)).bits.bit7

#define PTIM1_ETR            0x5253
#define TIM1_ETR             (*((volatile DR42*)PTIM1_ETR)).byte
#define TIM1_ETR_ETF         (*((volatile DR42*)PTIM1_ETR)).bits.bit03
#define TIM1_ETR_ETPS        (*((volatile DR42*)PTIM1_ETR)).bits.bit45
#define TIM1_ETR_ECE         (*((volatile DR42*)PTIM1_ETR)).bits.bit6
#define TIM1_ETR_ETP         (*((volatile DR42*)PTIM1_ETR)).bits.bit7

#define PTIM1_IER            0x5254
#define TIM1_IER             (*((volatile DR8*)PTIM1_IER)).byte
#define TIM1_IER_UIE         (*((volatile DR8*)PTIM1_IER)).bits.bit0
#define TIM1_IER_CC1IE       (*((volatile DR8*)PTIM1_IER)).bits.bit1
#define TIM1_IER_CC2IE       (*((volatile DR8*)PTIM1_IER)).bits.bit2
#define TIM1_IER_CC3IE       (*((volatile DR8*)PTIM1_IER)).bits.bit3
#define TIM1_IER_CC4IE       (*((volatile DR8*)PTIM1_IER)).bits.bit4
#define TIM1_IER_COMIE       (*((volatile DR8*)PTIM1_IER)).bits.bit5
#define TIM1_IER_TIE         (*((volatile DR8*)PTIM1_IER)).bits.bit6
#define TIM1_IER_BIE         (*((volatile DR8*)PTIM1_IER)).bits.bit7

#define PTIM1_SR1            0x5255
#define TIM1_SR1             (*((volatile DR8*)PTIM1_SR1)).byte
#define TIM1_SR1_UIF         (*((volatile DR8*)PTIM1_SR1)).bits.bit0
#define TIM1_SR1_CC1IF       (*((volatile DR8*)PTIM1_SR1)).bits.bit1
#define TIM1_SR1_CC2IF       (*((volatile DR8*)PTIM1_SR1)).bits.bit2
#define TIM1_SR1_CC3IF       (*((volatile DR8*)PTIM1_SR1)).bits.bit3
#define TIM1_SR1_CC4IF       (*((volatile DR8*)PTIM1_SR1)).bits.bit4
#define TIM1_SR1_COMIF       (*((volatile DR8*)PTIM1_SR1)).bits.bit5
#define TIM1_SR1_TIF         (*((volatile DR8*)PTIM1_SR1)).bits.bit6
#define TIM1_SR1_BIF         (*((volatile DR8*)PTIM1_SR1)).bits.bit7

#define PTIM1_SR2            0x5256
#define TIM1_SR2             (*((volatile DR8*)PTIM1_SR2)).byte
#define TIM1_SR2_CC1OF       (*((volatile DR8*)PTIM1_SR2)).bits.bit1
#define TIM1_SR2_CC2OF       (*((volatile DR8*)PTIM1_SR2)).bits.bit2
#define TIM1_SR2_CC3OF       (*((volatile DR8*)PTIM1_SR2)).bits.bit3
#define TIM1_SR2_CC4OF       (*((volatile DR8*)PTIM1_SR2)).bits.bit4

#define PTIM1_EGR            0x5257
#define TIM1_EGR             (*((volatile DR8*)PTIM1_EGR)).byte
#define TIM1_EGR_UG          (*((volatile DR8*)PTIM1_EGR)).bits.bit0
#define TIM1_EGR_CC1G        (*((volatile DR8*)PTIM1_EGR)).bits.bit1
#define TIM1_EGR_CC2G        (*((volatile DR8*)PTIM1_EGR)).bits.bit2
#define TIM1_EGR_CC3G        (*((volatile DR8*)PTIM1_EGR)).bits.bit3
#define TIM1_EGR_CC4G        (*((volatile DR8*)PTIM1_EGR)).bits.bit4
#define TIM1_EGR_COMG        (*((volatile DR8*)PTIM1_EGR)).bits.bit5
#define TIM1_EGR_TG          (*((volatile DR8*)PTIM1_EGR)).bits.bit6
#define TIM1_EGR_BG          (*((volatile DR8*)PTIM1_EGR)).bits.bit7

#define PTIM1_CCMR1          0x5258
#define TIM1_CCMR1           (*((volatile DR21131*)PTIM1_CCMR1)).byte
#define TIM1_CCMR1_CC1S      (*((volatile DR21131*)PTIM1_CCMR1)).bits.bit01 /* output & input */
#define TIM1_CCMR1_OC1FE     (*((volatile DR21131*)PTIM1_CCMR1)).bits.bit2  /* output */
#define TIM1_CCMR1_OC1PE     (*((volatile DR21131*)PTIM1_CCMR1)).bits.bit3  /* output */
#define TIM1_CCMR1_OC1M      (*((volatile DR21131*)PTIM1_CCMR1)).bits.bit46 /* output */
#define TIM1_CCMR1_OC1CE     (*((volatile DR21131*)PTIM1_CCMR1)).bits.bit7  /* output */
#define TIM1_CCMR1_IC1PSC    (*((volatile DR224*)PTIM1_CCMR1)).bits.bit23   /* input  */
#define TIM1_CCMR1_IC1F      (*((volatile DR224*)PTIM1_CCMR1)).bits.bit47   /* input  */

#define PTIM1_CCMR2          0x5259
#define TIM1_CCMR2           (*((volatile DR21131*)PTIM1_CCMR2)).byte
#define TIM1_CCMR2_CC2S      (*((volatile DR21131*)PTIM1_CCMR2)).bits.bit01 /* output & input */
#define TIM1_CCMR2_OC2FE     (*((volatile DR21131*)PTIM1_CCMR2)).bits.bit2  /* output */
#define TIM1_CCMR2_OC2PE     (*((volatile DR21131*)PTIM1_CCMR2)).bits.bit3  /* output */
#define TIM1_CCMR2_OC2M      (*((volatile DR21131*)PTIM1_CCMR2)).bits.bit46 /* output */
#define TIM1_CCMR2_OC2CE     (*((volatile DR21131*)PTIM1_CCMR2)).bits.bit7  /* output */
#define TIM1_CCMR2_IC2PSC    (*((volatile DR224*)PTIM1_CCMR2)).bits.bit23   /* input  */
#define TIM1_CCMR2_IC2F      (*((volatile DR224*)PTIM1_CCMR2)).bits.bit47   /* input  */

#define PTIM1_CCMR3          0x525A
#define TIM1_CCMR3           (*((volatile DR21131*)PTIM1_CCMR3)).byte
#define TIM1_CCMR3_CC3S      (*((volatile DR21131*)PTIM1_CCMR3)).bits.bit01 /* output & input */
#define TIM1_CCMR3_OC3FE     (*((volatile DR21131*)PTIM1_CCMR3)).bits.bit2  /* output */
#define TIM1_CCMR3_OC3PE     (*((volatile DR21131*)PTIM1_CCMR3)).bits.bit3  /* output */
#define TIM1_CCMR3_OC3M      (*((volatile DR21131*)PTIM1_CCMR3)).bits.bit46 /* output */
#define TIM1_CCMR3_OC3CE     (*((volatile DR21131*)PTIM1_CCMR3)).bits.bit7  /* output */
#define TIM1_CCMR3_IC3PSC    (*((volatile DR224*)PTIM1_CCMR3)).bits.bit23   /* input  */
#define TIM1_CCMR3_IC3F      (*((volatile DR224*)PTIM1_CCMR3)).bits.bit47   /* input  */

#define PTIM1_CCMR4          0x525B
#define TIM1_CCMR4           (*((volatile DR21131*)PTIM1_CCMR4)).byte
#define TIM1_CCMR4_CC4S      (*((volatile DR21131*)PTIM1_CCMR4)).bits.bit01 /* output & input */
#define TIM1_CCMR4_OC4FE     (*((volatile DR21131*)PTIM1_CCMR4)).bits.bit2  /* output */
#define TIM1_CCMR4_OC4PE     (*((volatile DR21131*)PTIM1_CCMR4)).bits.bit3  /* output */
#define TIM1_CCMR4_OC4M      (*((volatile DR21131*)PTIM1_CCMR4)).bits.bit46 /* output */
#define TIM1_CCMR4_OC4CE     (*((volatile DR21131*)PTIM1_CCMR4)).bits.bit7  /* output */
#define TIM1_CCMR4_IC4PSC    (*((volatile DR224*)PTIM1_CCMR4)).bits.bit23   /* input  */
#define TIM1_CCMR4_IC4F      (*((volatile DR224*)PTIM1_CCMR4)).bits.bit47   /* input  */

#define PTIM1_CCER1          0x525C
#define TIM1_CCER1           (*((volatile DR8*)PTIM1_CCER1)).byte
#define TIM1_CCER1_CC1E      (*((volatile DR8*)PTIM1_CCER1)).bits.bit0
#define TIM1_CCER1_CC1P      (*((volatile DR8*)PTIM1_CCER1)).bits.bit1
#define TIM1_CCER1_CC1NE     (*((volatile DR8*)PTIM1_CCER1)).bits.bit2
#define TIM1_CCER1_CC1NP     (*((volatile DR8*)PTIM1_CCER1)).bits.bit3
#define TIM1_CCER1_CC2E      (*((volatile DR8*)PTIM1_CCER1)).bits.bit4
#define TIM1_CCER1_CC2P      (*((volatile DR8*)PTIM1_CCER1)).bits.bit5
#define TIM1_CCER1_CC2NE     (*((volatile DR8*)PTIM1_CCER1)).bits.bit6
#define TIM1_CCER1_CC2NP     (*((volatile DR8*)PTIM1_CCER1)).bits.bit7

#define PTIM1_CCER2          0x525D
#define TIM1_CCER2           (*((volatile DR8*)PTIM1_CCER2)).byte
#define TIM1_CCER2_CC3E      (*((volatile DR8*)PTIM1_CCER2)).bits.bit0
#define TIM1_CCER2_CC3P      (*((volatile DR8*)PTIM1_CCER2)).bits.bit1
#define TIM1_CCER2_CC3NE     (*((volatile DR8*)PTIM1_CCER2)).bits.bit2
#define TIM1_CCER2_CC3NP     (*((volatile DR8*)PTIM1_CCER2)).bits.bit3
#define TIM1_CCER2_CC4E      (*((volatile DR8*)PTIM1_CCER2)).bits.bit4
#define TIM1_CCER2_CC4P      (*((volatile DR8*)PTIM1_CCER2)).bits.bit5

#define PTIM1_CNTRH          0x525E
#define TIM1_CNTRH           (*((volatile DR8*)PTIM1_CNTRH)).byte

#define PTIM1_CNTRL          0x525F
#define TIM1_CNTRL           (*((volatile DR8*)PTIM1_CNTRL)).byte

#define PTIM1_PSCRH          0x5260
#define TIM1_PSCRH           (*((volatile DR8*)PTIM1_PSCRH)).byte

#define PTIM1_PSCRL          0x5261
#define TIM1_PSCRL           (*((volatile DR8*)PTIM1_PSCRL)).byte

#define PTIM1_ARRH           0x5262
#define TIM1_ARRH            (*((volatile DR8*)PTIM1_ARRH)).byte

#define PTIM1_ARRL           0x5263
#define TIM1_ARRL            (*((volatile DR8*)PTIM1_ARRL)).byte

#define PTIM1_RCR            0x5264
#define TIM1_RCR             (*((volatile DR8*)PTIM1_RCR)).byte

#define PTIM1_CCR1H          0x5265
#define TIM1_CCR1H           (*((volatile DR8*)PTIM1_CCR1H)).byte

#define PTIM1_CCR1L          0x5266
#define TIM1_CCR1L           (*((volatile DR8*)PTIM1_CCR1L)).byte

#define PTIM1_CCR2H          0x5267
#define TIM1_CCR2H           (*((volatile DR8*)PTIM1_CCR2H)).byte

#define PTIM1_CCR2L          0x5268
#define TIM1_CCR2L           (*((volatile DR8*)PTIM1_CCR2L)).byte

#define PTIM1_CCR3H          0x5269
#define TIM1_CCR3H           (*((volatile DR8*)PTIM1_CCR3H)).byte

#define PTIM1_CCR3L          0x526A
#define TIM1_CCR3L           (*((volatile DR8*)PTIM1_CCR3L)).byte

#define PTIM1_CCR4H          0x526B
#define TIM1_CCR4H           (*((volatile DR8*)PTIM1_CCR4H)).byte

#define PTIM1_CCR4L          0x526C
#define TIM1_CCR4L           (*((volatile DR8*)PTIM1_CCR4L)).byte

#define PTIM1_BKR            0x526D
#define TIM1_BKR             (*((volatile DR21*)PTIM1_BKR)).byte
#define TIM1_BKR_LOCK        (*((volatile DR21*)PTIM1_BKR)).bits.bit01
#define TIM1_BKR_OSSI        (*((volatile DR21*)PTIM1_BKR)).bits.bit2
#define TIM1_BKR_OSSR        (*((volatile DR21*)PTIM1_BKR)).bits.bit3
#define TIM1_BKR_BKE         (*((volatile DR21*)PTIM1_BKR)).bits.bit4
#define TIM1_BKR_BKP         (*((volatile DR21*)PTIM1_BKR)).bits.bit5
#define TIM1_BKR_AOE         (*((volatile DR21*)PTIM1_BKR)).bits.bit6
#define TIM1_BKR_MOE         (*((volatile DR21*)PTIM1_BKR)).bits.bit7

#define PTIM1_DTR            0x526E
#define TIM1_DTR             (*((volatile DR8*)PTIM1_DTR)).byte

#define PTIM1_OISR           0x526F
#define TIM1_OISR            (*((volatile DR8*)PTIM1_OISR)).byte
#define TIM1_OISR_OIS1       (*((volatile DR8*)PTIM1_OISR)).bits.bit0
#define TIM1_OISR_OIS1N      (*((volatile DR8*)PTIM1_OISR)).bits.bit1
#define TIM1_OISR_OIS2       (*((volatile DR8*)PTIM1_OISR)).bits.bit2
#define TIM1_OISR_OIS2N      (*((volatile DR8*)PTIM1_OISR)).bits.bit3
#define TIM1_OISR_OIS3       (*((volatile DR8*)PTIM1_OISR)).bits.bit4
#define TIM1_OISR_OIS3N      (*((volatile DR8*)PTIM1_OISR)).bits.bit5
#define TIM1_OISR_OIS4       (*((volatile DR8*)PTIM1_OISR)).bits.bit6

//------------------------------------------------------------------------
// TIM2 (16-bit general purpose timer) register definitions
//------------------------------------------------------------------------
#define PTIM2_CR1            0x5300
#define TIM2_CR1             (*((volatile DR8*)PTIM2_CR1)).byte
#define TIM2_CR1_CEN         (*((volatile DR8*)PTIM2_CR1)).bits.bit0
#define TIM2_CR1_UDIS        (*((volatile DR8*)PTIM2_CR1)).bits.bit1
#define TIM2_CR1_URS         (*((volatile DR8*)PTIM2_CR1)).bits.bit2
#define TIM2_CR1_OPM         (*((volatile DR8*)PTIM2_CR1)).bits.bit3
#define TIM2_CR1_ARPE        (*((volatile DR8*)PTIM2_CR1)).bits.bit7

#define PTIM2_IER            0x5301
#define TIM2_IER             (*((volatile DR8*)PTIM2_IER)).byte
#define TIM2_IER_UIE         (*((volatile DR8*)PTIM2_IER)).bits.bit0
#define TIM2_IER_CC1IE       (*((volatile DR8*)PTIM2_IER)).bits.bit1
#define TIM2_IER_CC2IE       (*((volatile DR8*)PTIM2_IER)).bits.bit2
#define TIM2_IER_CC3IE       (*((volatile DR8*)PTIM2_IER)).bits.bit3
#define TIM2_IER_TIE         (*((volatile DR8*)PTIM2_IER)).bits.bit6

#define PTIM2_SR1            0x5302
#define TIM2_SR1             (*((volatile DR8*)PTIM2_SR1)).byte
#define TIM2_SR1_UIF         (*((volatile DR8*)PTIM2_SR1)).bits.bit0
#define TIM2_SR1_CC1IF       (*((volatile DR8*)PTIM2_SR1)).bits.bit1
#define TIM2_SR1_CC2IF       (*((volatile DR8*)PTIM2_SR1)).bits.bit2
#define TIM2_SR1_CC3IF       (*((volatile DR8*)PTIM2_SR1)).bits.bit3
#define TIM2_SR1_TIF         (*((volatile DR8*)PTIM2_SR1)).bits.bit6

#define PTIM2_SR2            0x5303
#define TIM2_SR2             (*((volatile DR8*)PTIM2_SR2)).byte
#define TIM2_SR2_CC1OF       (*((volatile DR8*)PTIM2_SR2)).bits.bit1
#define TIM2_SR2_CC2OF       (*((volatile DR8*)PTIM2_SR2)).bits.bit2
#define TIM2_SR2_CC3OF       (*((volatile DR8*)PTIM2_SR2)).bits.bit3

#define PTIM2_EGR            0x5304
#define TIM2_EGR             (*((volatile DR8*)PTIM2_EGR)).byte
#define TIM2_EGR_UG          (*((volatile DR8*)PTIM2_EGR)).bits.bit0
#define TIM2_EGR_CC1G        (*((volatile DR8*)PTIM2_EGR)).bits.bit1
#define TIM2_EGR_CC2G        (*((volatile DR8*)PTIM2_EGR)).bits.bit2
#define TIM2_EGR_CC3G        (*((volatile DR8*)PTIM2_EGR)).bits.bit3
#define TIM2_EGR_TG          (*((volatile DR8*)PTIM2_EGR)).bits.bit6

#define PTIM2_CCMR1          0x5305
#define TIM2_CCMR1           (*((volatile DR21131*)PTIM2_CCMR1)).byte
#define TIM2_CCMR1_CC1S      (*((volatile DR21131*)PTIM2_CCMR1)).bits.bit01 /* output & input */
#define TIM2_CCMR1_OC1PE     (*((volatile DR21131*)PTIM2_CCMR1)).bits.bit3  /* output */
#define TIM2_CCMR1_OC1M      (*((volatile DR21131*)PTIM2_CCMR1)).bits.bit46 /* output */
#define TIM2_CCMR1_IC1PSC    (*((volatile DR224*)PTIM2_CCMR1)).bits.bit23   /* input  */
#define TIM2_CCMR1_IC1F      (*((volatile DR224*)PTIM2_CCMR1)).bits.bit47   /* input  */

#define PTIM2_CCMR2          0x5306
#define TIM2_CCMR2           (*((volatile DR21131*)PTIM2_CCMR2)).byte
#define TIM2_CCMR2_CC2S      (*((volatile DR21131*)PTIM2_CCMR2)).bits.bit01 /* output & input */
#define TIM2_CCMR2_OC2PE     (*((volatile DR21131*)PTIM2_CCMR2)).bits.bit3  /* output */
#define TIM2_CCMR2_OC2M      (*((volatile DR21131*)PTIM2_CCMR2)).bits.bit46 /* output */
#define TIM2_CCMR2_IC2PSC    (*((volatile DR224*)PTIM2_CCMR2)).bits.bit23   /* input  */
#define TIM2_CCMR2_IC2F      (*((volatile DR224*)PTIM2_CCMR2)).bits.bit47   /* input  */

#define PTIM2_CCMR3          0x5307
#define TIM2_CCMR3           (*((volatile DR21131*)PTIM2_CCMR3)).byte
#define TIM2_CCMR3_CC3S      (*((volatile DR21131*)PTIM2_CCMR3)).bits.bit01 /* output & input */
#define TIM2_CCMR3_OC3PE     (*((volatile DR21131*)PTIM2_CCMR3)).bits.bit3  /* output */
#define TIM2_CCMR3_OC3M      (*((volatile DR21131*)PTIM2_CCMR3)).bits.bit46 /* output */
#define TIM2_CCMR3_IC3PSC    (*((volatile DR224*)PTIM2_CCMR3)).bits.bit23   /* input  */
#define TIM2_CCMR3_IC3F      (*((volatile DR224*)PTIM2_CCMR3)).bits.bit47   /* input  */

#define PTIM2_CCER1          0x5308
#define TIM2_CCER1           (*((volatile DR8*)PTIM2_CCER1)).byte
#define TIM2_CCER1_CC1E      (*((volatile DR8*)PTIM2_CCER1)).bits.bit0
#define TIM2_CCER1_CC1P      (*((volatile DR8*)PTIM2_CCER1)).bits.bit1
#define TIM2_CCER1_CC2E      (*((volatile DR8*)PTIM2_CCER1)).bits.bit4
#define TIM2_CCER1_CC2P      (*((volatile DR8*)PTIM2_CCER1)).bits.bit5

#define PTIM2_CCER2          0x5309
#define TIM2_CCER2           (*((volatile DR8*)PTIM2_CCER2)).byte
#define TIM2_CCER2_CC3E      (*((volatile DR8*)PTIM2_CCER2)).bits.bit0
#define TIM2_CCER2_CC3P      (*((volatile DR8*)PTIM2_CCER2)).bits.bit1

#define PTIM2_CNTRH          0x530A
#define TIM2_CNTRH           (*((volatile DR8*)PTIM2_CNTRH)).byte

#define PTIM2_CNTRL          0x530B
#define TIM2_CNTRL           (*((volatile DR8*)PTIM2_CNTRL)).byte

#define PTIM2_PSCR           0x530C
#define TIM2_PSCR            (*((volatile DR41*)PTIM2_PSCR)).byte
#define TIM2_PSCR_PSC        (*((volatile DR41*)PTIM2_PSCR)).bits.bit03

#define PTIM2_ARRH           0x530D
#define TIM2_ARRH            (*((volatile DR8*)PTIM2_ARRH)).byte

#define PTIM2_ARRL           0x530E
#define TIM2_ARRL            (*((volatile DR8*)PTIM2_ARRL)).byte

#define PTIM2_CCR1H          0x530F
#define TIM2_CCR1H           (*((volatile DR8*)PTIM2_CCR1H)).byte

#define PTIM2_CCR1L          0x5310
#define TIM2_CCR1L           (*((volatile DR8*)PTIM2_CCR1L)).byte

#define PTIM2_CCR2H          0x5311
#define TIM2_CCR2H           (*((volatile DR8*)PTIM2_CCR2H)).byte

#define PTIM2_CCR2L          0x5312
#define TIM2_CCR2L           (*((volatile DR8*)PTIM2_CCR2L)).byte

#define PTIM2_CCR3H          0x5313
#define TIM2_CCR3H           (*((volatile DR8*)PTIM2_CCR3H)).byte

#define PTIM2_CCR3L          0x5314
#define TIM2_CCR3L           (*((volatile DR8*)PTIM2_CCR3L)).byte

//------------------------------------------------------------------------
// TIM3 (16-bit general purpose timer) register definitions
//------------------------------------------------------------------------
#define PTIM3_CR1            0x5320
#define TIM3_CR1             (*((volatile DR8*)PTIM3_CR1)).byte
#define TIM3_CR1_CEN         (*((volatile DR8*)PTIM3_CR1)).bits.bit0
#define TIM3_CR1_UDIS        (*((volatile DR8*)PTIM3_CR1)).bits.bit1
#define TIM3_CR1_URS         (*((volatile DR8*)PTIM3_CR1)).bits.bit2
#define TIM3_CR1_OPM         (*((volatile DR8*)PTIM3_CR1)).bits.bit3
#define TIM3_CR1_ARPE        (*((volatile DR8*)PTIM3_CR1)).bits.bit7

#define PTIM3_IER            0x5321
#define TIM3_IER             (*((volatile DR8*)PTIM3_IER)).byte
#define TIM3_IER_UIE         (*((volatile DR8*)PTIM3_IER)).bits.bit0
#define TIM3_IER_CC1IE       (*((volatile DR8*)PTIM3_IER)).bits.bit1
#define TIM3_IER_CC2IE       (*((volatile DR8*)PTIM3_IER)).bits.bit2
#define TIM3_IER_CC3IE       (*((volatile DR8*)PTIM3_IER)).bits.bit3
#define TIM3_IER_TIE         (*((volatile DR8*)PTIM3_IER)).bits.bit6

#define PTIM3_SR1            0x5322
#define TIM3_SR1             (*((volatile DR8*)PTIM3_SR1)).byte
#define TIM3_SR1_UIF         (*((volatile DR8*)PTIM3_SR1)).bits.bit0
#define TIM3_SR1_CC1IF       (*((volatile DR8*)PTIM3_SR1)).bits.bit1
#define TIM3_SR1_CC2IF       (*((volatile DR8*)PTIM3_SR1)).bits.bit2
#define TIM3_SR1_CC3IF       (*((volatile DR8*)PTIM3_SR1)).bits.bit3
#define TIM3_SR1_TIF         (*((volatile DR8*)PTIM3_SR1)).bits.bit6

#define PTIM3_SR2            0x5323
#define TIM3_SR2             (*((volatile DR8*)PTIM3_SR2)).byte
#define TIM3_SR2_CC1OF       (*((volatile DR8*)PTIM3_SR2)).bits.bit1
#define TIM3_SR2_CC2OF       (*((volatile DR8*)PTIM3_SR2)).bits.bit2
#define TIM3_SR2_CC3OF       (*((volatile DR8*)PTIM3_SR2)).bits.bit3

#define PTIM3_EGR            0x5324
#define TIM3_EGR             (*((volatile DR8*)PTIM3_EGR)).byte
#define TIM3_EGR_UG          (*((volatile DR8*)PTIM3_EGR)).bits.bit0
#define TIM3_EGR_CC1G        (*((volatile DR8*)PTIM3_EGR)).bits.bit1
#define TIM3_EGR_CC2G        (*((volatile DR8*)PTIM3_EGR)).bits.bit2
#define TIM3_EGR_CC3G        (*((volatile DR8*)PTIM3_EGR)).bits.bit3
#define TIM3_EGR_TG          (*((volatile DR8*)PTIM3_EGR)).bits.bit6

#define PTIM3_CCMR1          0x5325
#define TIM3_CCMR1           (*((volatile DR21131*)PTIM3_CCMR1)).byte
#define TIM3_CCMR1_CC1S      (*((volatile DR21131*)PTIM3_CCMR1)).bits.bit01 /* output & input */
#define TIM3_CCMR1_OC1PE     (*((volatile DR21131*)PTIM3_CCMR1)).bits.bit3  /* output */
#define TIM3_CCMR1_OC1M      (*((volatile DR21131*)PTIM3_CCMR1)).bits.bit46 /* output */
#define TIM3_CCMR1_IC1PSC    (*((volatile DR224*)PTIM3_CCMR1)).bits.bit23   /* input  */
#define TIM3_CCMR1_IC1F      (*((volatile DR224*)PTIM3_CCMR1)).bits.bit47   /* input  */

#define PTIM3_CCMR2          0x5326
#define TIM3_CCMR2           (*((volatile DR21131*)PTIM3_CCMR2)).byte
#define TIM3_CCMR2_CC2S      (*((volatile DR21131*)PTIM3_CCMR2)).bits.bit01 /* output & input */
#define TIM3_CCMR2_OC2PE     (*((volatile DR21131*)PTIM3_CCMR2)).bits.bit3  /* output */
#define TIM3_CCMR2_OC2M      (*((volatile DR21131*)PTIM3_CCMR2)).bits.bit46 /* output */
#define TIM3_CCMR2_IC2PSC    (*((volatile DR224*)PTIM3_CCMR2)).bits.bit23   /* input  */
#define TIM3_CCMR2_IC2F      (*((volatile DR224*)PTIM3_CCMR2)).bits.bit47   /* input  */

#define PTIM3_CCER1          0x5327
#define TIM3_CCER1           (*((volatile DR8*)PTIM3_CCER1)).byte
#define TIM3_CCER1_CC1E      (*((volatile DR8*)PTIM3_CCER1)).bits.bit0
#define TIM3_CCER1_CC1P      (*((volatile DR8*)PTIM3_CCER1)).bits.bit1
#define TIM3_CCER1_CC2E      (*((volatile DR8*)PTIM3_CCER1)).bits.bit4
#define TIM3_CCER1_CC2P      (*((volatile DR8*)PTIM3_CCER1)).bits.bit5

#define PTIM3_CNTRH          0x5328
#define TIM3_CNTRH           (*((volatile DR8*)PTIM3_CNTRH)).byte

#define PTIM3_CNTRL          0x5329
#define TIM3_CNTRL           (*((volatile DR8*)PTIM3_CNTRL)).byte

#define PTIM3_PSCR           0x532A
#define TIM3_PSCR            (*((volatile DR41*)PTIM3_PSCR)).byte
#define TIM3_PSCR_PSC        (*((volatile DR41*)PTIM3_PSCR)).bits.bit03

#define PTIM3_ARRH           0x532B
#define TIM3_ARRH            (*((volatile DR8*)PTIM3_ARRH)).byte

#define PTIM3_ARRL           0x532C
#define TIM3_ARRL            (*((volatile DR8*)PTIM3_ARRL)).byte
#define PTIM3_CCR1H          0x532D

#define TIM3_CCR1H           (*((volatile DR8*)PTIM3_CCR1H)).byte

#define PTIM3_CCR1L          0x532E
#define TIM3_CCR1L           (*((volatile DR8*)PTIM3_CCR1L)).byte

#define PTIM3_CCR2H          0x532F
#define TIM3_CCR2H           (*((volatile DR8*)PTIM3_CCR2H)).byte

#define PTIM3_CCR2L          0x5330
#define TIM3_CCR2L           (*((volatile DR8*)PTIM3_CCR2L)).byte

//------------------------------------------------------------------------
// TIM4 (8-bit basic timer) register definitions
//------------------------------------------------------------------------
#define PTIM4_CR1            0x5340
#define TIM4_CR1             (*((volatile DR8*)PTIM4_CR1)).byte
#define TIM4_CR1_CEN         (*((volatile DR8*)PTIM4_CR1)).bits.bit0
#define TIM4_CR1_UDIS        (*((volatile DR8*)PTIM4_CR1)).bits.bit1
#define TIM4_CR1_URS         (*((volatile DR8*)PTIM4_CR1)).bits.bit2
#define TIM4_CR1_OPM         (*((volatile DR8*)PTIM4_CR1)).bits.bit3
#define TIM4_CR1_ARPE        (*((volatile DR8*)PTIM4_CR1)).bits.bit7

#define PTIM4_IER            0x5341
#define TIM4_IER             (*((volatile DR8*)PTIM4_IER)).byte
#define TIM4_IER_UIE         (*((volatile DR8*)PTIM4_IER)).bits.bit0
#define TIM4_IER_TIE         (*((volatile DR8*)PTIM4_IER)).bits.bit6

#define PTIM4_SR1            0x5342
#define TIM4_SR1             (*((volatile DR8*)PTIM4_SR1)).byte
#define TIM4_SR1_UIF         (*((volatile DR8*)PTIM4_SR1)).bits.bit0
#define TIM4_SR1_TIF         (*((volatile DR8*)PTIM4_SR1)).bits.bit6

#define PTIM4_EGR            0x5343
#define TIM4_EGR             (*((volatile DR8*)PTIM4_EGR)).byte
#define TIM4_EGR_UG          (*((volatile DR8*)PTIM4_EGR)).bits.bit0
#define TIM4_EGR_TG          (*((volatile DR8*)PTIM4_EGR)).bits.bit6

#define PTIM4_CNTR           0x5344
#define TIM4_CNTR            (*((volatile DR8*)PTIM4_CNTR)).byte

#define PTIM4_PSCR           0x5345
#define TIM4_PSCR            (*((volatile DR3131*)PTIM4_PSCR)).byte
#define TIM4_PSCR_PSC        (*((volatile DR3131*)PTIM4_PSCR)).bits.bit02

#define PTIM4_ARR            0x5346
#define TIM4_ARR             (*((volatile DR8*)PTIM4_ARR)).byte

//------------------------------------------------------------------------
// ADC1 (analog/digital converter) register definitions
//------------------------------------------------------------------------
#define PADC_DB0RH           0x53E0
#define ADC_DB0RH            (*((volatile DR8*)PADC_DB0RH)).byte

#define PADC_DB0RL           0x53E1
#define ADC_DB0RL            (*((volatile DR8*)PADC_DB0RL)).byte

#define PADC_DB1RH           0x53E2
#define ADC_DB1RH            (*((volatile DR8*)PADC_DB1RH)).byte

#define PADC_DB1RL           0x53E3
#define ADC_DB1RL            (*((volatile DR8*)PADC_DB1RL)).byte

#define PADC_DB2RH           0x53E4
#define ADC_DB2RH            (*((volatile DR8*)PADC_DB2RH)).byte

#define PADC_DB2RL           0x53E5
#define ADC_DB2RL            (*((volatile DR8*)PADC_DB2RL)).byte

#define PADC_DB3RH           0x53E6
#define ADC_DB3RH            (*((volatile DR8*)PADC_DB3RH)).byte

#define PADC_DB3RL           0x53E7
#define ADC_DB3RL            (*((volatile DR8*)PADC_DB3RL)).byte

#define PADC_DB4RH           0x53E8
#define ADC_DB4RH            (*((volatile DR8*)PADC_DB4RH)).byte

#define PADC_DB4RL           0x53E9
#define ADC_DB4RL            (*((volatile DR8*)PADC_DB4RL)).byte

#define PADC_DB5RH           0x53EA
#define ADC_DB5RH            (*((volatile DR8*)PADC_DB5RH)).byte

#define PADC_DB5RL           0x53EB
#define ADC_DB5RL            (*((volatile DR8*)PADC_DB5RL)).byte

#define PADC_DB6RH           0x53EC
#define ADC_DB6RH            (*((volatile DR8*)PADC_DB6RH)).byte

#define PADC_DB6RL           0x53ED
#define ADC_DB6RL            (*((volatile DR8*)PADC_DB6RL)).byte

#define PADC_DB7RH           0x53EE
#define ADC_DB7RH            (*((volatile DR8*)PADC_DB7RH)).byte

#define PADC_DB7RL           0x53EF
#define ADC_DB7RL            (*((volatile DR8*)PADC_DB7RL)).byte

#define PADC_DB8RH           0x53F0
#define ADC_DB8RH            (*((volatile DR8*)PADC_DB8RH)).byte

#define PADC_DB8RL           0x53F1
#define ADC_DB8RL            (*((volatile DR8*)PADC_DB8RL)).byte

#define PADC_DB9RH           0x53F2
#define ADC_DB9RH            (*((volatile DR8*)PADC_DB9RH)).byte

#define PADC_DB9RL           0x53F3
#define ADC_DB9RL            (*((volatile DR8*)PADC_DB9RL)).byte

#define PADC_CSR             0x5400
#define ADC_CSR              (*((volatile DR41*)PADC_CSR)).byte
#define ADC_CSR_CH           (*((volatile DR41*)PADC_CSR)).bits.bit03
#define ADC_CSR_AWDIE        (*((volatile DR41*)PADC_CSR)).bits.bit4
#define ADC_CSR_EOCIE        (*((volatile DR41*)PADC_CSR)).bits.bit5
#define ADC_CSR_AWD          (*((volatile DR41*)PADC_CSR)).bits.bit6
#define ADC_CSR_EOC          (*((volatile DR41*)PADC_CSR)).bits.bit7

#define PADC_CR1             0x5401
#define ADC_CR1              (*((volatile DR111131*)PADC_CR1)).byte
#define ADC_CR1_ADON         (*((volatile DR111131*)PADC_CR1)).bits.bit0
#define ADC_CR1_CONT         (*((volatile DR111131*)PADC_CR1)).bits.bit1
#define ADC_CR1_SPSEL        (*((volatile DR111131*)PADC_CR1)).bits.bit46

#define PADC_CR2             0x5402
#define ADC_CR2              (*((volatile DR111121*)PADC_CR2)).byte
#define ADC_CR2_SCAN         (*((volatile DR111121*)PADC_CR2)).bits.bit1
#define ADC_CR2_ALIGN        (*((volatile DR111121*)PADC_CR2)).bits.bit3
#define ADC_CR2_EXTSEL       (*((volatile DR111121*)PADC_CR2)).bits.bit45
#define ADC_CR2_EXTTRIG      (*((volatile DR111121*)PADC_CR2)).bits.bit6

#define PADC_CR3             0x5403
#define ADC_CR3              (*((volatile DR8*)PADC_CR3)).byte
#define ADC_CR3_OVR          (*((volatile DR8*)PADC_CR3)).bits.bit6
#define ADC_CR3_DBUF         (*((volatile DR8*)PADC_CR3)).bits.bit7

#define PADC_DRH             0x5404
#define ADC_DRH              (*((volatile DR8*)PADC_DRH)).byte

#define PADC_DRL             0x5405
#define ADC_DRL              (*((volatile DR8*)PADC_DRL)).byte

#define PADC_TDRH            0x5406
#define ADC_TDRH             (*((volatile DR8*)PADC_TDRH)).byte

#define PADC_TDRL            0x5407
#define ADC_TDRL             (*((volatile DR8*)PADC_TDRL)).byte

#define PADC_HTRH            0x5408
#define ADC_HTRH             (*((volatile DR8*)PADC_HTRH)).byte

#define PADC_HTRL            0x5409
#define ADC_HTRL             (*((volatile DR21*)PADC_HTRL)).byte
#define ADC_HTRL_HT          (*((volatile DR21*)PADC_HTRL)).bits.bit01

#define PADC_LTRH            0x540A
#define ADC_LTRH             (*((volatile DR8*)PADC_LTRH)).byte

#define PADC_LTRL            0x540B
#define ADC_LTRL             (*((volatile DR21*)PADC_LTRL)).byte
#define ADC_LTRL_LT          (*((volatile DR21*)PADC_LTRL)).bit01

#define PADC_AWSRH           0x540C
#define ADC_AWSRH            (*((volatile DR8*)PADC_AWSRH)).byte
#define ADC_AWSRH_AWS8       (*((volatile DR8*)PADC_AWSRH)).bits.bit0
#define ADC_AWSRH_AWS9       (*((volatile DR8*)PADC_AWSRH)).bits.bit1

#define PADC_AWSRL           0x540D
#define ADC_AWSRL            (*((volatile DR8*)PADC_AWSRL)).byte
#define ADC_AWSRL_AWS0       (*((volatile DR8*)PADC_AWSRL)).bit0
#define ADC_AWSRL_AWS1       (*((volatile DR8*)PADC_AWSRL)).bit1
#define ADC_AWSRL_AWS2       (*((volatile DR8*)PADC_AWSRL)).bit2
#define ADC_AWSRL_AWS3       (*((volatile DR8*)PADC_AWSRL)).bit3
#define ADC_AWSRL_AWS4       (*((volatile DR8*)PADC_AWSRL)).bit4
#define ADC_AWSRL_AWS5       (*((volatile DR8*)PADC_AWSRL)).bit5
#define ADC_AWSRL_AWS6       (*((volatile DR8*)PADC_AWSRL)).bit6
#define ADC_AWSRL_AWS7       (*((volatile DR8*)PADC_AWSRL)).bit7

#define PADC_AWCRH           0x540E
#define ADC_AWCRH            (*((volatile DR8*)PADC_AWCRH)).byte
#define ADC_AWCRH_AWEN8      (*((volatile DR8*)PADC_AWCRH)).bits.bit0
#define ADC_AWCRH_AWEN9      (*((volatile DR8*)PADC_AWCRH)).bits.bit1

#define PADC_AWCRL           0x540F
#define ADC_AWCRL            (*((volatile DR8*)PADC_AWCRL)).byte
#define ADC_AWCRL_AWEN0      (*((volatile DR8*)PADC_AWCRL)).bits.bit0
#define ADC_AWCRL_AWEN1      (*((volatile DR8*)PADC_AWCRL)).bits.bit1
#define ADC_AWCRL_AWEN2      (*((volatile DR8*)PADC_AWCRL)).bits.bit2
#define ADC_AWCRL_AWEN3      (*((volatile DR8*)PADC_AWCRL)).bits.bit3
#define ADC_AWCRL_AWEN4      (*((volatile DR8*)PADC_AWCRL)).bits.bit4
#define ADC_AWCRL_AWEN5      (*((volatile DR8*)PADC_AWCRL)).bits.bit5
#define ADC_AWCRL_AWEN6      (*((volatile DR8*)PADC_AWCRL)).bits.bit6
#define ADC_AWCRL_AWEN7      (*((volatile DR8*)PADC_AWCRL)).bits.bit7

//------------------------------------------------------------------------
// CPU register definitions
//------------------------------------------------------------------------
#define PCPU_A               0x7F00
#define CPU_A                (*((volatile DR8*)PCPU_A)).byte   /* Accumulator */

#define PCPU_PCE             0x7F01
#define CPU_PCE              (*((volatile DR8*)PCPU_PCE)).byte /* Program counter extended */

#define PCPU_PCH             0x7F02
#define CPU_PCH              (*((volatile DR8*)PCPU_PCH)).byte /* Program counter high */

#define PCPU_PCL             0x7F03
#define CPU_PCL              (*((volatile DR8*)PCPU_PCL)).byte /* Program counter low */

#define PCPU_XH              0x7F04
#define CPU_XH               (*((volatile DR8*)PCPU_XH)).byte  /* X index register high */

#define PCPU_XL              0x7F05
#define CPU_XL               (*((volatile DR8*)PCPU_XL)).byte  /* X index register low */

#define PCPU_YH              0x7F06
#define CPU_YH               (*((volatile DR8*)PCPU_YH)).byte  /* Y index register high */

#define PCPU_YL              0x7F07
#define CPU_YL               (*((volatile DR8*)PCPU_YL)).byte  /* Y index register low */

#define PCPU_SPH             0x7F08
#define CPU_SPH              (*((volatile DR8*)PCPU_SPH)).byte  /* Stack pointer high */

#define PCPU_SPL             0x7F09
#define CPU_SPL              (*((volatile DR8*)PCPU_SPL)).byte  /* Stack pointer low */

#define PCPU_CCR             0x7F0A
#define CPU_CCR              (*((volatile DR8*)PCPU_CCR)).byte  /* Condition code register */
#define CPU_CCR_C            (*((volatile DR8*)PCPU_CCR)).bits.bit0
#define CPU_CCR_Z            (*((volatile DR8*)PCPU_CCR)).bits.bit1
#define CPU_CCR_NF           (*((volatile DR8*)PCPU_CCR)).bits.bit2
#define CPU_CCR_I0           (*((volatile DR8*)PCPU_CCR)).bits.bit3
#define CPU_CCR_H            (*((volatile DR8*)PCPU_CCR)).bits.bit4
#define CPU_CCR_I1           (*((volatile DR8*)PCPU_CCR)).bits.bit5
#define CPU_CCR_V            (*((volatile DR8*)PCPU_CCR)).bits.bit7

#define PCPU_CFG_GCR         0x7F60
#define CPU_CFG_GCR          (*((volatile DR8*)PCPU_CFG_GCR)).byte  /* Global configuration register */
#define CPU_CFG_GCR_SWO      (*((volatile DR8*)PCPU_CFG_GCR)).bits.bit0
#define CPU_CFG_GCR_AL       (*((volatile DR8*)PCPU_CFG_GCR)).bits.bit1

//------------------------------------------------------------------------
// ITC - SPR register definitions
//------------------------------------------------------------------------
#define PITC_SPR1            0x7F70
#define ITC_SPR1             (*((volatile DR2222*)PITC_SPR1)).byte  /* Interrupt SW priority register 1 */
#define ITC_SPR1_VECT0SPR    (*((volatile DR2222*)PITC_SPR1)).bit01
#define ITC_SPR1_VECT1SPR    (*((volatile DR2222*)PITC_SPR1)).bit23
#define ITC_SPR1_VECT2SPR    (*((volatile DR2222*)PITC_SPR1)).bit45
#define ITC_SPR1_VECT3SPR    (*((volatile DR2222*)PITC_SPR1)).bit67

#define PITC_SPR2            0x7F71
#define ITC_SPR2             (*((volatile DR2222*)PITC_SPR2)).byte  /* Interrupt SW priority register 2 */
#define ITC_SPR2_VECT4SPR    (*((volatile DR2222*)PITC_SPR2)).bit01
#define ITC_SPR2_VECT5SPR    (*((volatile DR2222*)PITC_SPR2)).bit23
#define ITC_SPR2_VECT6SPR    (*((volatile DR2222*)PITC_SPR2)).bit45
#define ITC_SPR2_VECT7SPR    (*((volatile DR2222*)PITC_SPR2)).bit67

#define PITC_SPR3            0x7F72
#define ITC_SPR3             (*((volatile DR2222*)PITC_SPR3)).byte  /* Interrupt SW priority register 3 */
#define ITC_SPR3_VECT8SPR    (*((volatile DR2222*)PITC_SPR3)).bit01
#define ITC_SPR3_VECT9SPR    (*((volatile DR2222*)PITC_SPR3)).bit23
#define ITC_SPR3_VECT10SPR   (*((volatile DR2222*)PITC_SPR3)).bit45
#define ITC_SPR3_VECT11SPR   (*((volatile DR2222*)PITC_SPR3)).bit67

#define PITC_SPR4            0x7F73
#define ITC_SPR4             (*((volatile DR2222*)PITC_SPR4)).byte  /* Interrupt SW priority register 4 */
#define ITC_SPR4_VECT12SPR   (*((volatile DR2222*)PITC_SPR4)).bit01
#define ITC_SPR4_VECT13SPR   (*((volatile DR2222*)PITC_SPR4)).bit23
#define ITC_SPR4_VECT14SPR   (*((volatile DR2222*)PITC_SPR4)).bit45
#define ITC_SPR4_VECT15SPR   (*((volatile DR2222*)PITC_SPR4)).bit67

#define PITC_SPR5            0x7F74
#define ITC_SPR5             (*((volatile DR2222*)PITC_SPR5)).byte  /* Interrupt SW priority register 5 */
#define ITC_SPR5_VECT16SPR   (*((volatile DR2222*)PITC_SPR5)).bit01
#define ITC_SPR5_VECT17SPR   (*((volatile DR2222*)PITC_SPR5)).bit23
#define ITC_SPR5_VECT18SPR   (*((volatile DR2222*)PITC_SPR5)).bit45
#define ITC_SPR5_VECT19SPR   (*((volatile DR2222*)PITC_SPR5)).bit67

#define PITC_SPR6            0x7F75
#define ITC_SPR6             (*((volatile DR2222*)PITC_SPR6)).byte  /* Interrupt SW priority register 6 */
#define ITC_SPR6_VECT20SPR   (*((volatile DR2222*)PITC_SPR6)).bit01
#define ITC_SPR6_VECT21SPR   (*((volatile DR2222*)PITC_SPR6)).bit23
#define ITC_SPR6_VECT22SPR   (*((volatile DR2222*)PITC_SPR6)).bit45
#define ITC_SPR6_VECT23SPR   (*((volatile DR2222*)PITC_SPR6)).bit67

#define PITC_SPR7            0x7F76
#define ITC_SPR7             (*((volatile DR2222*)PITC_SPR7)).byte  /* Interrupt SW priority register 7 */
#define ITC_SPR7_VECT24SPR   (*((volatile DR2222*)PITC_SPR7)).bit01
#define ITC_SPR7_VECT25SPR   (*((volatile DR2222*)PITC_SPR7)).bit23
#define ITC_SPR7_VECT26SPR   (*((volatile DR2222*)PITC_SPR7)).bit45
#define ITC_SPR7_VECT27SPR   (*((volatile DR2222*)PITC_SPR7)).bit67

#define PITC_SPR8            0x7F77
#define ITC_SPR8             (*((volatile DR2222*)PITC_SPR8)).byte  /* Interrupt SW priority register 8 */
#define ITC_SPR8_VECT28SPR   (*((volatile DR2222*)PITC_SPR8)).bit01
#define ITC_SPR8_VECT29SPR   (*((volatile DR2222*)PITC_SPR8)).bit23

//------------------------------------------------------------------------
// SWIM register definitions
//------------------------------------------------------------------------
#define PSWIM_CSR            0x7F80
#define SWIM_CSR             (*((volatile DR8*)PSWIM_CSR)).byte  /* SWIM control status register */

//------------------------------------------------------------------------
// DM register definitions
//------------------------------------------------------------------------
#define PDM_BK1RE            0x7F90
#define DM_BK1RE             (*((volatile DR8*)PDM_BK1RE)).byte  /* DM breakpoint 1 register extended byte */

#define PDM_BK1RH            0x7F91
#define DM_BK1RH             (*((volatile DR8*)PDM_BK1RH)).byte  /* DM breakpoint 1 register high byte */

#define PDM_BK1RL            0x7F92
#define DM_BK1RL             (*((volatile DR8*)PDM_BK1RL)).byte  /* DM breakpoint 1 register low byte */

#define PDM_BK2RE            0x7F93
#define DM_BK2RE             (*((volatile DR8*)PDM_BK2RE)).byte  /* DM breakpoint 2 register extended byte */

#define PDM_BK2RH            0x7F94
#define DM_BK2RH             (*((volatile DR8*)PDM_BK2RH)).byte  /* DM breakpoint 2 register high byte */

#define PDM_BK2RL            0x7F95
#define DM_BK2RL             (*((volatile DR8*)PDM_BK2RL)).byte  /* DM breakpoint 2 register low byte */

#define PDM_CR1              0x7F96
#define DM_CR1               (*((volatile DR8*)PDM_CR1)).byte    /* DM debug module control register 1 */

#define PDM_CR2              0x7F97
#define DM_CR2               (*((volatile DR8*)PDM_CR2)).byte    /* DM debug module control register 2 */

#define PDM_CSR1             0x7F98
#define DM_CSR1              (*((volatile DR8*)PDM_CSR1)).byte   /* DM debug module control/status register 1 */

#define PDM_CSR2             0x7F99
#define DM_CSR2              (*((volatile DR8*)PDM_CSR2)).byte   /* DM debug module control/status register 2 */

#define PDM_ENFCTR           0x7F9A
#define DM_ENFCTR            (*((volatile DR8*)PDM_ENFCTR)).byte /* DM enable function register */

//------------------------------------------------------------------------
// Interrupt numbers, taken from the stm8s105 datasheet. For IAR: add +2.
//------------------------------------------------------------------------
#define AWU_vector                            1 /* AWU Auto Wake up from Halt */
#define CLK_CSS_vector                        2 /* CLK Security System */
#define CLK_SWITCH_vector                     2 /* Master clock switch event */
#define EXTI0_vector                          3 /* EXTI0 Port A external interrupts */
#define EXTI1_vector                          4 /* EXTI1 Port B external interrupts */
#define EXTI2_vector                          5 /* EXTI2 Port C external interrupts */
#define EXTI3_vector                          6 /* EXTI3 Port D external interrupts */
#define EXTI4_vector                          7 /* EXTI4 Port E external interrupts */
#define SPI_CRCERR_vector                    10 /* SPI CRC error interrupt */
#define SPI_MODF_vector                      10 /* SPI Mode fault error interrupt */
#define SPI_OVR_vector                       10 /* SPI Overrun fault error interrupt*/
#define SPI_RXNE_vector                      10 /* SPI Receive buffer not empty error interrupt */
#define SPI_TXE_vector                       10 /* SPI Transmit buffer empty error interrupt */
#define SPI_WKUP_vector                      10 /* SPI Wakeup interrupt */
#define TIM1_OVR_BIF_vector                  11 /* Timer 1 Break interrupt */
#define TIM1_OVR_TIF_vector                  11 /* Timer 1 Trigger interrupt */
#define TIM1_OVR_UIF_vector                  11 /* Timer 1 Update interrupt */
#define TIM1_CAPCOM_CC1IF_vector             12 /* Timer 1 Capture/compare 1 interrupt */
#define TIM1_CAPCOM_CC2IF_vector             12 /* Timer 1 Capture/compare 2 interrupt */
#define TIM1_CAPCOM_CC3IF_vector             12 /* Timer 1 Capture/compare 3 interrupt */
#define TIM1_CAPCOM_CC4IF_vector             12 /* Timer 1 Capture/compare 4 interrupt */
#define TIM1_CAPCOM_COMIF_vector             12 /* Timer 1 Capture/compare 1 interrupt */
#define TIM2_OVR_UIF_vector                  13 /* Timer 2 Update interrupt */
#define TIM3_OVR_UIF_vector                  15 /* Timer 3 Update interrupt */
#define TIM2_CAPCOM_CC1IF_vector             14 /* Timer 2 Capture/compare 1 interrupt */
#define TIM2_CAPCOM_CC2IF_vector             14 /* Timer 2 Capture/compare 2 interrupt */
#define TIM2_CAPCOM_CC3IF_vector             14 /* Timer 2 Capture/compare 3 interrupt */
#define TIM2_CAPCOM_TIF_vector               14 /* Timer 2 Trigger interrupt */
#define TIM3_CAPCOM_CC1IF_vector             16 /* Timer 3 Capture/compare 1 interrupt */
#define TIM3_CAPCOM_CC2IF_vector             16 /* Timer 3 Capture/compare 2 interrupt */
#define TIM3_CAPCOM_CC3IF_vector             16 /* Timer 3 Capture/compare 3 interrupt */
#define TIM3_CAPCOM_TIF_vector               16 /* Timer 3 Trigger interrupt */
#define I2C_ADD10_vector                     19 /* I2C 10-bit header sent interrupt */
#define I2C_ADDR_vector                      19 /* I2C Address sent/matched interrupt */
#define I2C_AF_vector                        19 /* I2C Ack failure interrupt */
#define I2C_ARLO_vector                      19 /* I2C Arbitration loss interrupt */
#define I2C_BERR_vector                      19 /* I2C Bus error interrupt */
#define I2C_BTF_vector                       19 /* I2C Dat byte transfer finished interrupt */
#define I2C_OVR_vector                       19 /* I2C Overrun/underrun interrupt */
#define I2C_RXNE_vector                      19 /* I2C Receive buffer not empty interrupt */
#define I2C_SB_vector                        19 /* I2C Start bit sent interrupt */
#define I2C_STOPF_vector                     19 /* I2C Stop received interrupt */
#define I2C_TXE_vector                       19 /* I2C Transmit buffer empty interrupt */
#define I2C_WUFH_vector                      19 /* I2C Wakeup from Halt interrupt */
#define UART2_T_TC_vector                    20 /* UART Transmission complete interrupt */
#define UART2_T_TXE_vector                   20 /* UART Transmit data register empty interrupt */
#define UART2_R_IDLE_vector                  21 /* UART Idle line detected interrupt */
#define UART2_R_LBDF_vector                  21 /* UART Break flag interrupt */
#define UART2_R_OR_vector                    21 /* UART Overrun detected interrupt */
#define UART2_R_PE_vector                    21 /* UART Parity error interrupt */
#define UART2_R_RXNE_vector                  21 /* UART Received data ready to be read */
#define ADC1_AWDG_vector                     22 /* ADC Analog watchdog interrupt */
#define ADC1_AWS0_vector                     22 /* ADC Interrupt */
#define ADC1_AWS1_vector                     22 /* ADC Interrupt */
#define ADC1_AWS2_vector                     22 /* ADC Interrupt */
#define ADC1_AWS3_vector                     22 /* ADC Interrupt */
#define ADC1_AWS4_vector                     22 /* ADC Interrupt */
#define ADC1_AWS5_vector                     22 /* ADC Interrupt */
#define ADC1_AWS6_vector                     22 /* ADC Interrupt */
#define ADC1_AWS7_vector                     22 /* ADC Interrupt */
#define ADC1_AWS8_vector                     22 /* ADC Interrupt */
#define ADC1_AWS9_vector                     22 /* ADC Interrupt */
#define ADC1_EOC_vector                      22 /* ADC End-of-Conversion interrupt */
#define TIM4_OVR_UIF_vector                  23 /* Timer 4 Update interrupt */
#define FLASH_EOP_vector                     24 /* Flash End-of-Programming interrupt */
#define FLASH_WR_PG_DIS_vector               24 /* Flash Write attempt to protected page interrupt */

#endif /* USE_IAR_HEADER_FILE */
#endif /* _STH8S105_H_ */

//------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------
