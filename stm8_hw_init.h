#ifndef _STM8_HW_INIT_H
#define _STM8_HW_INIT_H
/*===================================================================================
  File Name: stm8_hw_init.h
  Author   : Emile
  ----------------------------------------------------------------------------------
  Purpose  : this is the header file for stm8_hw_init.c.
             It is based on RGB_platform_v03 PCB.
  ----------------------------------------------------------------------------------
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
  ----------------------------------------------------------------------------------
                        STM8S105C6T6 HW version 01
      MCU pin-name            Function |    MCU pin-name        Function
   ------------------------------------|--------------------------------
   01 NRST                    -        | 13 VDDA
   02 PA1/OSC                 -        | 14 VSSA
   03 PA2/OSCOUT              OK       | 15 PB7/AIN7            KHZ
   04 VSSIO_1                          | 16 PB6/AIN6            HZ
   05 VSS                              | 17 PB5/AIN5[I2C_SDA]   PERC
   06 VCAP                             | 18 PB4/AIN4[I2C_SCL]   DB
   07 VDD                              | 19 PB3/AIN3[TIM1_ETR]  DIST 
   08 VDDIO_1                          | 20 PB2/AIN2[TIM1_CH3N] LVL_IN
   09 PA3/TIM2_CH3[TIME3_CH1] RIGHT    | 21 PB1/AIN1[TIM1_CH2N] LVL_OUT
   10 PA4                     LEFT     | 22 PB0/AIN0[TIM1_CH1N] -
   11 PA5                     DOWN     | 23 PE7/AIN8            BG_LED
   12 PA6                     UP       | 24 PE6/AIN9            VPK
   ------------------------------------|--------------------------------
   25 PE5/SPI_NSS             VRMS     | 37 PE3/TIM1_BKIN       CLK4
   26 PC1/TIM1_CH1/UART2_CK   FREQ     | 38 PE2/I2C_SDA         SDA
   27 PC2/TIM1_CH2            -        | 39 PE1/I2C_SCL         SCL
   28 PC3/TIM1_CH3            STCP     | 40 PE0/CLK_CC0         DIO4
   29 PC4/TIM1_CH4            SHCP     | 41 PD0/TIM3_CH2...     ISR
   30 PC5/SPI_SCK             SDIN1    | 42 PD1/SWIM            SWIM
   31 VSSIO_2                          | 43 PD2/TIM3_CH1...     DIO2 
   32 VDDIO_2                          | 44 PD3/TIM2_CH2...     CLK2
   33 PC6/SPI_MOSI            SDIN2    | 45 PD4/TIM2_CH1[BEEP]  DIO3
   34 PC7/SPI_MISO            SDIN3    | 46 PD5/UART2_TX        TX
   35 PG0                     DIO1     | 47 PD6/UART2_RX        RX
   36 PG1                     CLK1     | 48 PD7/TLI[TIM1_CH4]   CLK3
  ======================================================================*/
#include "stm8s105.h"
#include <stdint.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// PORT A defines
//-----------------------------------------------------------------------------
#define UP       (0x40) /* UP Key */
#define DOWN     (0x20) /* DOWN Key */
#define LEFT     (0x10) /* LEFT Key */
#define RIGHT    (0x08) /* RIGHT Key */
#define OK       (0x04) /* OK Key */
#define ALL_KEYS (UP | DOWN | LEFT | RIGHT | OK)

//-----------------------------------------------------------------------------
// PORT B defines
//-----------------------------------------------------------------------------
#define KHZ      (0x80) /* PB7, kHz Led */
#define HZ       (0x40) /* PB6, Hz Led */
#define PERC     (0x20) /* PB5, Percentage Led */
#define DB       (0x10) /* PB4, dB Led */

#define ALL_LEDS (KHZ | HZ | PERC | DB)

#define ADC3     (3) /* PB3/AIN3 is ADC3: Distortion   */
#define ADC2     (2) /* PB2/AIN2 is ADC2: Input  Level */
#define ADC1     (1) /* PB1/AIN1 is ADC1: Output Level */
       
//-----------------------------------------------------------------------------
// ADC1: DC-value of generated sine-wave. Adjust P192 in such a way that
//       1 V amplitude (1 Vp) on AD_MAIN results in 0.6 Volts on ADC1.
//       A full-scale value for the sine-wave is 5 Vrms or 5*sqrt(2) Vp.
//       This results in 3*sqrt(2) = 4.24264 V on ADC1 or 868 decimal.
//       ADC1 Full-scale Vp value is therefore 5000 (mV) * (5/3) / 1023 = 8.1459759
//       Example: 868 * 8.1459759 = 7071 mVp
//       Full-scale RMS value is therefore 8.1459759 / sqrt(2) = 5.7600748
//       Example: 868 * 5.7600748 = 5000 mVrms
//       Note: the ADC will overflow above 8.33 Vp, 16.67 Vpp or 5.89 Vrms.
//-----------------------------------------------------------------------------
#define ADC1_FS_VPK  ( 8.1459759) /* ADC1 Full-scale Vpeak voltage */
#define ADC1_FS_VRMS ( 5.7600748) /* ADC1 Full-scale Vrms  voltage */
#define ADC1_FS_VPP  (16.2919518) /* ADC1 Full-scale Vpp   voltage */

// use these defines to directly control the output-pins
#define KHZb     (PB_ODR_ODR7)
#define HZb      (PB_ODR_ODR6)
#define PERCb    (PB_ODR_ODR5)
#define DBb      (PB_ODR_ODR4)

//-----------------------------------------------------------------------------
// PORT C defines
//-----------------------------------------------------------------------------
#define SDIN3       (0x80) /* PC7: SDIN3 for HC595 PCB1 */
#define SDIN2       (0x40) /* PC5: SDIN2 for HC595 PCB2 */
#define SDIN1       (0x20) /* PC5: SDIN1 for HC595 PCB1 */
#define SHCP        (0x10) /* PC4: STCP for HC595 */
#define STCP        (0x08) /* PC3: STCP for HC595 */
#define FREQ        (0x02) /* PC1: FREQ counter input on TIM1_CH1 */       

// use these defines to directly control the output-pins
#define SDIN3b      (PC_ODR_ODR7) /* Serial Data 3 */
#define SDIN2b      (PC_ODR_ODR6) /* Serial Data 2 */ 
#define SDIN1b      (PC_ODR_ODR5) /* Serial Data 1 */
#define SHCPb       (PC_ODR_ODR4) /* clock for HC595 shift-registers */
#define STCPb       (PC_ODR_ODR3) /* clock for HC595 output-registers */
#define PC_CR2_FREQ (PC_CR2_C21)  /* Actual frequency input */

//-----------------------------------------------------------------------------
// PORT D defines
//-----------------------------------------------------------------------------
#define CLK3        (0x80)  /* PD7: CLK for TM1637 SSD3: Distortion display */
// PD6 (RX) & PD5 (TX) are initialized by the UART module
#define DIO3        (0x10)  /* PD4: DIO for TM1637 SSD3: Distortion display */
#define CLK2        (0x08)  /* PD3: CLK for TM1637 SSD2: Output Level display */
#define DIO2        (0x04)  /* PD2: DIO for TM1637 SSD2: Output Level display */
// PD1 (SWIM) is initialized by the JTAG module
#define IRQ_LED     (0x01)  /* PD0, shows interrupt status */

// use these defines to directly control the output-pins
#define CLK3b      (PD_ODR_ODR7)
#define DIO3b      (PD_ODR_ODR4)
#define CLK2b      (PD_ODR_ODR3)
#define DIO2b      (PD_ODR_ODR2)
#define IRQ_LEDb   (PD_ODR_ODR0)

//-----------------------------------------------------------------------------
// PORT E defines
//-----------------------------------------------------------------------------
#define BG_LED      (0x80) /* PE7, shows background process status */
#define VPK_LED     (0x40) /* PE6, Vpeak LED */
#define VRMS_LED    (0x20) /* PE5, Vrms LED */
#define CLK4        (0x08) /* PE3: CLK for TM1637 SSD4: Input Level display */
#define I2C_SDA     (0x04) /* PE2: I2C SDA */
#define I2C_SCL     (0x02) /* PE1: I2C SCL */
#define DIO4        (0x01) /* PE0: DIO for TM1637 SSD4: Input Level display */

// use these defines to directly control the output-pins
#define BG_LEDb    (PE_ODR_ODR7)
#define VPK_LEDb   (PE_ODR_ODR6)
#define VRMS_LEDb  (PE_ODR_ODR5)
#define CLK4b      (PE_ODR_ODR3)
#define DIO4b      (PE_ODR_ODR0)

//-----------------------------------------------------------------------------
// PORT G defines
//-----------------------------------------------------------------------------
#define CLK1        (0x02) /* PG1: CLK for TM1637 SSD1: Frequency display */
#define DIO1        (0x01) /* PG0: DIO for TM1637 SSD1: Frequency display */

// use these defines to directly control the output-pins
#define CLK1b      (PG_ODR_ODR1)
#define DIO1b      (PG_ODR_ODR0)

//-----------------------------
// Timer 2 modes
//-----------------------------
#define FREQ_1KHZ (0)
#define FREQ_2KHZ (1)
#define FREQ_4KHZ (2)
#define FREQ_8KHZ (3)

//-----------------------------------------------------------------------------
// Timer definitions for calc_freq()
//-----------------------------------------------------------------------------
#define CLK_400_KHZ             ( 400000) /* Clock for 20 Hz..200 Hz measurements     */
#define CLK_4_MHZ               (4000000) /* Clock for 250 Hz..2 kHz measurements     */
#define CLK_PERIOD_MEAS_MIN     (   1000) /* half of min. nr. of clock-ticks          */
#define CLK_TICKS_MIN           (    800) /* 20% below lowest valid measurement       */
#define CLK_TICKS_MAX           (   2000) /* highest number with no display overflow  */
#define CLK_TICKS_100K_MIN      (   8000) /* min. clock-ticks for 100 kHz measurement */
#define CLK_TICKS_100K_MAX      (  12000) /* max. clock-ticks for 100 kHz measurement */
#define CLK_TICKS_13K_80K_MIN   (   1000) /* min. clock-ticks for 13 kHz measurement  */
#define CLK_TICKS_13K_80K_MAX   (   9999) /* max. clock-ticks for 80 kHz measurement  */
#define CLK_TICKS_130K_200K_MIN (  10000) /* min. clock-ticks for 130 kHz measurement */
#define CLK_TICKS_130K_200K_MAX (  22000) /* max. clock-ticks for 200 kHz measurement */

//-----------------------------------------
// Definitions for seven-segment displays
//-----------------------------------------
#define SSD_FREQ    (0) /* Actual frequency of generated sine-wave */
#define SSD_LVL_OUT (1) /* Amplitude of generated sine-wave [VRMS,VPK,VPP] */
#define SSD_DIST    (2) /* Actual distortion level */
#define SSD_LVL_IN  (3) /* Amplitude of incoming sine-wave [VRMS,VPK,VPP] */

//-------------------------------------------------------------
// Bit-definitions for UP, DOWN, LEFT, RIGHT and OK in buttons
//-------------------------------------------------------------
#define BTN_UP	  (0x1010)
#define BTN_DOWN  (0x0808)
#define BTN_LEFT  (0x0404)
#define BTN_RIGHT (0x0202)
#define BTN_OK    (0x0101)
#define BTN_ANY   (0x1F1F)
      
//-------------------------------------------------------------
// Helpful definitions to handle buttons
//-------------------------------------------------------------
#define BTN_IDLE(btn)		  ((buttons & (btn)) == 0x0000)
#define BTN_PRESSED(btn)	  ((buttons & (btn)) == ((btn) & 0x001F))
#define BTN_HELD(btn)		  ((buttons & (btn)) == (btn))
#define BTN_RELEASED(btn)	  ((buttons & (btn)) == ((btn) & 0x1F00))
      
// Function prototypes
void     buzzer(void);
void     set_buzzer(uint8_t freq, uint8_t nrbeeps);
void     initialise_system_clock(void);
void     calc_freq(void);
void     setup_timer1(uint8_t f);
void     setup_timer2(uint8_t freq);
void     read_buttons(void);
void     setup_gpio_ports(void);
#endif // _STM8_HW_INIT_H