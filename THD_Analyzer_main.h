/*==================================================================
  File Name    : THD_Analyzer_main.h
  Author       : Emile
  ------------------------------------------------------------------
  This is the header file for THD_Analyzer_main.c, which is the main-body
  for the control board of the THD Analyzer.
  This version is made for the STM8S105C6T6 uC.

  This file is part of the THD-Analyzer project.
  ------------------------------------------------------------------
  This is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this file.  If not, see <http://www.gnu.org/licenses/>.
  ==================================================================*/
#ifndef __THD_ANALYZER_MAIN_H__
#define __THD_ANALYZER_MAIN_H__

#include "stm8s105.h"
#include "stm8_hw_init.h"
#include <stdint.h>
#include "delay.h"

#define MAX_BITS12 (24) /* 3 x HC595 = 24 bits for PCB1 and PCB2 */
#define MAX_BITS3  (16) /* 2 x HC595 = 16 bits for PCB3 */

#define NOSEND (false) /* do NOT send to HC595 */
#define SEND   (true)  /* send bits to HC595 hardware */

//--------------------------------------------------------------------------
// The first 14 bits have the same meaning on every PCB:
// Bit 13: 1 = Range 200 kHz        Bit 06: 1 = freq 100 Hz
// Bit 12: 1 = Range  20 kHz        Bit 05: 1 = freq  80 Hz
// Bit 11: 1 = Range   2 kHz        Bit 04: 1 = freq  65 Hz
// Bit 10: 1 = Range 200  Hz        Bit 03: 1 = freq  50 Hz
// Bit 09: 1 = freq  200  Hz        Bit 02: 1 = freq  40 Hz
// Bit 08: 1 = freq  160  Hz        Bit 01: 1 = freq  30 Hz
// Bit 07: 1 = freq  130  Hz        Bit 00: 1 = freq  25 Hz
//--------------------------------------------------------------------------
// The other bits in pcb1_bits are meant for PCB1 and have the following meaning:
// Bit 23: 1 = LVL 1.5 V or 5 V     Bit 18: not used
// Bit 22: 1 = LVL 0.5 V            Bit 17: not used
// Bit 21: 1 = LVL 0.15 V           Bit 16: Range  20 kHz
// Bit 20: 1 = LVL != OFF           Bit 15: Range   2 kHz
// Bit 19: 1 = LVL 5 V              Bit 14: Range 200  Hz
//--------------------------------------------------------------------------
// The other bits in pcb2_bits are meant for PCB2 and have the following meaning:
// Bit 23: 1 = INP 100 V              Bit 18: 1 = SENS 0.1%, 1% or 10%
// Bit 22: 1 = INP 30 V               Bit 17: 1 = SENS 0.03%, 0.3% or 3%
// Bit 21: 1 = INP 10 V               Bit 16: 1 = SENS 0.01 %
// Bit 20: 1 = INP 3 V                Bit 15: 1 = SENS 0.003 %
// Bit 19: 1 = INP 1 V                Bit 14: 1 = SENS < 3 %
//--------------------------------------------------------------------------
// The bits in pcb3_bits are meant for PCB3 and have the following meaning:
// Bit 15: 1 = SENS < 0.3 %           Bit 14: 1 = SENS > 0.01 %
//--------------------------------------------------------------------------

// The RANGE mask is the same for every PCB,
// but PCB1 has some additional RANGE bits.
#define RANGE_200_HZ        (0)
#define RANGE_2_KHZ         (1)
#define RANGE_20_KHZ        (2)
#define RANGE_200_KHZ       (3)
#define RANGE_200_HZ_MASK   (0x00000400)
#define RANGE_2_KHZ_MASK    (0x00000800)
#define RANGE_20_KHZ_MASK   (0x00001000)
#define RANGE_200_KHZ_MASK  (0x00002000) /* same for alle PCBs */
#define RANGE_MASK          (0x00003C00)
#define RANGE1_200_HZ_MASK  (0x00004400)
#define RANGE1_2_KHZ_MASK   (0x00008800)
#define RANGE1_20_KHZ_MASK  (0x00011000)
#define RANGE1_MASK         (0x0001FC00)

#define FREQ_20_HZ    (0) /* RANGE 200 Hz */
#define FREQ_25_HZ    (1)
#define FREQ_30_HZ    (2)
#define FREQ_40_HZ    (3)
#define FREQ_50_HZ    (4)
#define FREQ_65_HZ    (5)
#define FREQ_80_HZ    (6)
#define FREQ_100_HZ   (7)
#define FREQ_130_HZ   (8)
#define FREQ_160_HZ   (9)
#define FREQ_200_HZ  (10)
#define FREQ_250_HZ  (11) /* RANGE 2 kHz */
#define FREQ_300_HZ  (12)
#define FREQ_400_HZ  (13)
#define FREQ_500_HZ  (14)
#define FREQ_650_HZ  (15)
#define FREQ_800_HZ  (16)
#define FREQ_1000_HZ (17)
#define FREQ_1300_HZ (18)
#define FREQ_1600_HZ (19)
#define FREQ_2000_HZ (20)
#define FREQ_2500_HZ (21)  /* RANGE 20 kHz */
#define FREQ_3000_HZ (22)
#define FREQ_4000_HZ (23)
#define FREQ_5000_HZ (24)
#define FREQ_6500_HZ (25)
#define FREQ_8000_HZ (26)
#define FREQ_10_KHZ  (27)
#define FREQ_13_KHZ  (28)
#define FREQ_16_KHZ  (29)
#define FREQ_20_KHZ  (30)
#define FREQ_25_KHZ  (31) /* RANGE 200 kHz */
#define FREQ_30_KHZ  (32)
#define FREQ_40_KHZ  (33)
#define FREQ_50_KHZ  (34)
#define FREQ_65_KHZ  (35)
#define FREQ_80_KHZ  (36)
#define FREQ_100_KHZ (37)
#define FREQ_130_KHZ (38)
#define FREQ_160_KHZ (39)
#define FREQ_200_KHZ (40)
#define FREQ_MASK    (0x000003FF)

//--------------------------------------------------------------------------
// The LEVEL bits are meant to set the output-level of the signal generator
// and are specific for PCB1. They cover bits 19 - 23.
//--------------------------------------------------------------------------
#define LEVEL_OFF       (0)
#define LEVEL_0V15      (1)
#define LEVEL_0V50      (2)
#define LEVEL_1V50      (3)
#define LEVEL_5V        (4)
#define LEVEL_0V15_MASK (0x00300000)
#define LEVEL_0V50_MASK (0x00500000)
#define LEVEL_1V50_MASK (0x00900000)
#define LEVEL_5V_MASK   (0x00980000)
#define LEVEL_MASK      (0x00F80000)

//--------------------------------------------------------------------------
// The INPUT bits are meant to control the input-level of the received
// signal and are specific for PCB2.
//--------------------------------------------------------------------------
#define INPUT_1V   (0)
#define INPUT_3V   (1)
#define INPUT_10V  (2)
#define INPUT_30V  (3)
#define INPUT_100V (4)
#define INPUT_MASK (0x00F80000)

//--------------------------------------------------------------------------
// The SENSITIVITY bits are meant to control the distortion level in %
// and are specific for PCB2 and PCB3.
//--------------------------------------------------------------------------
#define SENS_0_003  (0)
#define SENS_0_010  (1)
#define SENS_0_030  (2)
#define SENS_0_100  (3)
#define SENS_0_300  (4)
#define SENS_1_000  (5)
#define SENS_3_000  (6)
#define SENS_10_000 (7)
#define SENS_0_003_MASK  (0x0000C000)
#define SENS_0_010_MASK  (0x00014000)
#define SENS_0_030_MASK  (0x00024000)
#define SENS_0_100_MASK  (0x00044000)
#define SENS_0_300_MASK  (0x00024000)
#define SENS_1_000_MASK  (0x00044000)
#define SENS_3_000_MASK  (0x00020000)
#define SENS_10_000_MASK (0x00040000)
#define SENS_MASK_PCB2   (0x0007C000)
#define SENS_LT_0_300    (0x8000)
#define SENS_GT_0_010    (0x4000)
#define SENS_MASK_PCB3   (0xC000)

//--------------------------------------------------------------------------
// Defines for measured frequency on SSD
//--------------------------------------------------------------------------
#define DP0_HZ  (0) /* No decimals, frequency in Hz   [20 ,9999]  */
#define DP2_KHZ (1) /* Two decimals, frequency in kHz [10.00,99.99] */
#define DP1_KHZ (2) /* One decimal, frequency in kHz  [100.0,200.0] */

//-------------------------------------
// Defines for State Machine
//-------------------------------------
#define STD_IDLE     (0) /* Idle state */
#define STD_LVL_OUT  (1) /* Set Amplitude level outgoing sinewave */
#define STD_LVL_IN   (2) /* Set Amplitude level incoming signal */
#define STD_SENS     (3) /* Set sensitivity level */
#define STD_SWEEP    (4) /* Sweep through all frequencies */
#define STD_AMPL     (5) /* Change amplitude unity */
#define STD_DIST     (6) /* Change distortion unity */
#define STD_BRIGHT   (7) /* Change brightness of SSD displays */
#define TMR_NO_KEY (150) /* 15 sec. */
#define TMR_SWEEP   (20) /*  2 sec. */

//-------------------------------------
// Defines for Amplitude Measurement
//-------------------------------------
#define VRMS      (0)
#define VPEAK     (1)
#define VPP       (2)
#define DIST_PERC (false)
#define DIST_DB   (true)
    
//------------------------------------------
// Defines for individual leds next to SSDs
//------------------------------------------
#define LED_OFF  (0) 
#define LED_VPP  (1) /* Bit 0: digit 5, segment A */
#define LED_VPK  (2) /* Bit 1: digit 5, segment B */
#define LED_VRMS (4) /* Bit 2: digit 5, segment C */
#define LED_DB   (1) /* Bit 0: digit 5, segment A */
#define LED_PERC (2) /* Bit 1: digit 5, segment B */
#define LED_PPM  (4) /* Bit 2: digit 5, segment C */
#define LED_KHZ  (1) /* Bit 0: digit 5, segment A */
#define LED_HZ   (4) /* Bit 1: digit 5, segment C */
    
//-------------------------------------
// Function prototypes
//-------------------------------------
void adc_task(void);
void freq_task(void);
void ctrl_task(void);

uint16_t divu10(uint16_t n);
void     freqkHz(char *s);

void set_range(uint8_t range);
void set_frequency(uint8_t freq, bool send);
void set_output_level(uint8_t lvl, bool send);
void set_input_level(uint8_t lvl, bool send);
void set_sensitivity(uint8_t sens, bool send);
void send_to_hc595(void);
void eep_init(bool init);

#if defined(__SDCC)
    // List all interrupt functions here, see SDCC manual par.3.8.1:
    // If you have multiple source files in your project, interrupt service routines
    // can be present in any of them, but a prototype of the isr MUST be present or
    // included in the file that contains the function main.
    void PORTC_EXT_IRQ(void) __interrupt(EXTI2_vector);
    void TIM2_UPD_OVF_IRQHandler(void) __interrupt(TIM2_OVR_UIF_vector);
    void UART_TX_IRQHandler(void) __interrupt(UART2_T_TXE_vector);
    void UART_RX_IRQHandler(void) __interrupt(UART2_R_RXNE_vector);
#endif // defined

#endif // __THD_ANALYZER_MAIN_H__
