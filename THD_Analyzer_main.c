/*==================================================================
  File Name    : THD_Analyzer_main.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the main() function and all the
            hardware related functions for the STM8S105C6T6 uC.
            It is meant for the THD-Analyzer Control Board (PCB4).
  ------------------------------------------------------------------
  This file is free software: you can redistribute it and/or modify
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "thd_analyzer_main.h"
#include "scheduler.h"
#include "adc.h"
#include "eep.h"
#include "i2c_bb.h"
#include "comms.h"
#include "uart.h"
#include "lcd_i2c.h"
#include "tm1637.h"

// Version number for THD-Analyzer firmware
char version[]          = "THD-Control V0.27";
const char hz[10][3]    = {"20","25","30","40","50","65","80","10","13","16"};

int16_t  lvl_out_adc;   // Sine-wave output level, ADC1
int16_t  lvl_in_adc;    // Input-level, ADC2
int16_t  lvl_dist_adc;  // Distortion level, ADC3
bool     dist_db;       // Distortion level unity [DIST_PERC, DIST_DB]
uint16_t freq_meas;     // Actual measured frequency
uint16_t freq_meas_old; // Previous value of freq_meas
uint8_t  fmt_meas;      // Format for freq_meas

uint8_t  lvl_out = 0;          // [LEVEL_OFF, ..., LEVEL_5V]
uint8_t  lo_old = 0;           // Previous value of lvl_out
uint8_t  lvl_in = 0;           // [INPUT_1V, ..., INPUT_100V]
uint8_t  li_old = 0;           // Previous value of lvl_in
uint8_t  range;                // [RANGE_200_HZ, ..., RANGE_200_KHZ]
uint8_t  sensi = 0;            // [SENS_0_003, ..., SENS_10_000]
uint8_t  sensi_old = 0;        // Previous value of sensi
int8_t   amplitude = VPEAK;    // Amplitude in VPeak
int8_t   brightness = 1;       // Brightness of SSD displays

uint8_t  menustate = STD_IDLE;     // STD number
uint8_t  std_tmr;                  // countdown timer for STD
uint8_t  sweep_tmr;                // timer for frequency sweep
uint8_t  fsweep = FREQ_20_HZ;      // current sweep frequency

uint8_t  freq_sine = 0;            // [FREQ_20_HZ, ..., FREQ_200_KHZ]
uint8_t  freq_save = 0;            // freq_sine value just before sweep mode
uint8_t  freq_old = 0;             // Previous value of freq_sine
uint32_t pcb1_bits = 0L;           // 32 bits for PCB1 Shift-registers
uint32_t pcb2_bits = 0L;           // 32 bits for PCB2 Shift-registers
uint16_t pcb3_bits = 0x0000;       // 16 bits for PCB3 Shift-registers

float  lvl_in_c100;                // conversion constant for 100V input level
float  lvl_in_c30;                 // conversion constant for  30V input level
float  lvl_in_c10;                 // conversion constant for  10V input level
float  lvl_in_c3;                  // conversion constant for   3V input level
float  lvl_in_c1;                  // conversion constant for   1V input level

float  sensi_10c;                  // Sensitivity 10% constant
float  sensi_3c;                   // Sensitivity 3% constant 
float  sensi_1c;                   // Sensitivity 1% constant
float  sensi_c3;                   // Sensitivity 0.3% constant
float  sensi_c1;                   // Sensitivity 0.1% constant
float  sensi_c03;                  // Sensitivity 300 ppm constant
float  sensi_c01;                  // Sensitivity 100 ppm constant
float  sensi_c003;                 // Sensitivity 30 ppm constant

uint16_t buttons_aux1;             // Previous and Actual value of TM1637 navi-keys
uint16_t buttons_aux2;             // Previous and Actual value of TM1637 aux keys

extern char     rs232_inbuf[];
extern uint16_t buttons;           // Previous and Actual value of navi-keys

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Range of the sine-wave generator.
  Variables: r: range parameter
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_range(uint8_t r)
{
    range = r;                           // save range parameter
    pcb1_bits &= ~(uint32_t)RANGE1_MASK; // clear bits for RANGE
    pcb2_bits &= ~(uint32_t)RANGE_MASK;  // clear bits for RANGE
    pcb3_bits &= ~(uint16_t)RANGE_MASK;  // clear bits for RANGE
    switch (r)
    {
        case RANGE_200_HZ:
            pcb1_bits |= RANGE1_200_HZ_MASK; /* PCB1 has more RANGE bits */
            pcb2_bits |= RANGE_200_HZ_MASK;
            pcb3_bits |= (uint16_t)RANGE_200_HZ_MASK;
            break;
        case RANGE_2_KHZ:
            pcb1_bits |= RANGE1_2_KHZ_MASK;  /* PCB1 has more RANGE bits */
            pcb2_bits |= RANGE_2_KHZ_MASK;
            pcb3_bits |= (uint16_t)RANGE_2_KHZ_MASK;
            break;
        case RANGE_20_KHZ:
            pcb1_bits |= RANGE1_20_KHZ_MASK;  /* PCB1 has more RANGE bits */
            pcb2_bits |= RANGE_20_KHZ_MASK;
            pcb3_bits |= (uint16_t)RANGE_20_KHZ_MASK;
            break;
        default: // 200 kHz, not on PCB2
            pcb1_bits |= RANGE_200_KHZ_MASK; /* same for PCB1 and PCB3 */
            pcb3_bits |= (uint16_t)RANGE_200_KHZ_MASK;
            break;
    } // switch
} // set_range()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Frequency of the sine-wave generator.
  Variables: freq: frequency index [FREQ_20_HZ .. FREQ_200_KHZ]
             send: [NOSEND,SEND]
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_frequency(uint8_t freq, bool send)
{
    uint8_t f = freq;

    if (f < FREQ_250_HZ)
    {   // 20 Hz .. 200 Hz
        set_range(RANGE_200_HZ);
    } // if
    else if (f < FREQ_2500_HZ)
    {   // 250 Hz .. 2 kHz
        set_range(RANGE_2_KHZ);
        f -= FREQ_200_HZ;
    } // else if
    else if (f < FREQ_25_KHZ)
    {   // 2.5 kHz .. 20 kHz
        set_range(RANGE_20_KHZ);
        f -= FREQ_2000_HZ;
    } // else if
    else
    {   // 25 kHz .. 200 kHz
        set_range(RANGE_200_KHZ);
        f -= FREQ_20_KHZ;
    } // else
    pcb1_bits &= ~(uint32_t)FREQ_MASK; // clear all frequency bits
    pcb2_bits &= ~(uint32_t)FREQ_MASK; // clear all frequency bits
    pcb3_bits &= ~(uint16_t)FREQ_MASK; // clear all frequency bits
    if (freq > FREQ_20_HZ)
    {   // 20 Hz requires all relays to be off, all other freqs should be switched
        pcb1_bits |= (1L << (f-1)); // switch proper relay PCB1
        pcb2_bits |= (1L << (f-1)); // switch proper relay PCB2
        pcb3_bits |= (1  << (f-1)); // switch proper relay PCB3
    } // if
    freq_sine = freq;          // save frequency set
    if (send) send_to_hc595(); // send to hardware
} // set_frequency()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Output Level of
             the sine-wave generator on PCB1.
  Variables: lvl:  output level index [LEVEL_OFF .. LEVEL_5V]
             send: [NOSEND,SEND]
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_output_level(uint8_t lvl, bool send)
{
    pcb1_bits &= ~(uint32_t)LEVEL_MASK; // clear all level bits
    lvl_out = lvl;
    switch (lvl)
    {
        case LEVEL_0V15:
            pcb1_bits |= LEVEL_0V15_MASK;
            break;
        case LEVEL_0V50:
            pcb1_bits |= LEVEL_0V50_MASK;
            break;
        case LEVEL_1V50:
            pcb1_bits |= LEVEL_1V50_MASK;
            break;
        case LEVEL_5V:
            pcb1_bits |= LEVEL_5V_MASK;
            break;
        default:             // LEVEL_OFF, all bits 0, including the !OFF bit
            PC_CR2_FREQ = 0; // disable freq. meas. to prevent IRQ overloads
            break;
    } // switch
    if (send) send_to_hc595(); // send to hardware
} // set_output_level()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Input Level for PCB2.
  Variables: lvl:  input level index [INPUT_1V .. INPUT_100V]
             send: [NOSEND,SEND]
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_input_level(uint8_t lvl, bool send)
{
    lvl_in = lvl;
    if (lvl <= INPUT_100V)
    {
        pcb2_bits &= ~(uint32_t)INPUT_MASK; // clear all level bits
        pcb2_bits |= (1L << (19 + lvl));
    } // if
    if (send) send_to_hc595(); // send to hardware
} // set_input_level()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Sensitivity Level in % for PCB2 and PCB3
  Variables: send:  sensitivity level index [SENS_0_003 .. SENS_10_000]
             send: [NOSEND,SEND]
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_sensitivity(uint8_t sens, bool send)
{
    sensi = sens;
    pcb2_bits &= ~(uint32_t)SENS_MASK_PCB2; // clear all sensitivity bits
    pcb3_bits &= ~(uint16_t)SENS_MASK_PCB3; // clear all sensitivity bits
    switch (sens)
    {
        case SENS_0_003: // 0.003 %
            pcb2_bits |= SENS_0_003_MASK;
            break;
        case SENS_0_010: // 0.01 %
            pcb2_bits |= SENS_0_010_MASK;
            break;
        case SENS_0_030: // 0.03 %
            pcb2_bits |= SENS_0_030_MASK;
            break;
        case SENS_0_100: // 0.1 %
            pcb2_bits |= SENS_0_100_MASK;
            break;
        case SENS_0_300: // 0.3 %
            pcb2_bits |= SENS_0_300_MASK;
            break;
        case SENS_1_000: // 1 %
            pcb2_bits |= SENS_1_000_MASK;
            break;
        case SENS_3_000: // 3 %
            pcb2_bits |= SENS_3_000_MASK;
            break;
        default: // SENS = 10 %
            pcb2_bits |= SENS_10_000_MASK;
            break;
    } // switch
    if (sens < SENS_0_300) pcb3_bits |= SENS_LT_0_300;
    if (sens > SENS_0_010) pcb3_bits |= SENS_GT_0_010;
    if (send) send_to_hc595(); // send to hardware
} // set_sensitivity()

/*-----------------------------------------------------------------------------
  Purpose  : This function sends the bit-settings to the HC595 shift-registers
             on the 3 PCBs of the THD Analyzer.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void send_to_hc595(void)
{
    uint32_t colmask  = 0x00000001;  // start with bit 0 to send to shift-register
    uint16_t colmask3 = 0x0001;      // bit-mask for PCB3 shift-register
    uint8_t i = 0;

    //-------------------------------------------------------------------
    // Since PCB3 contains fewer shift-register bits than PCB1 and PCB2,
    // we first start by shifting the extra bits to PCB1 and PCB2. These
    // bits are shifted out again by the PCB3 shift-register.
    //-------------------------------------------------------------------
    while (i < MAX_BITS12 - MAX_BITS3)
    {   // shift bits to HC595 shift-registers
        SDIN1b    = (pcb1_bits & colmask) ? 1 : 0;
        SDIN2b    = (pcb2_bits & colmask) ? 1 : 0;
        SDIN3b    = 0; // dummy
        SHCPb     = 1; // set clock for shift-register;
        colmask <<= 1; // select next bit
        SHCPb     = 0; // set clock to 0 again
        i++;
    } // while
    colmask3 = 0x0001;
    while (i < MAX_BITS12)
    {   // shift MAX_BITS3 bits to all HC595 shift-registers
        SDIN1b     = (pcb1_bits & colmask)  ? 1 : 0;
        SDIN2b     = (pcb2_bits & colmask)  ? 1 : 0;
        SDIN3b     = (pcb3_bits & colmask3) ? 1 : 0;
        SHCPb      = 1; // set clock for shift-register;
        colmask  <<= 1; // select next bit for PCB1 and PCB2
        colmask3 <<= 1; // select next bit for PCB3
        SHCPb      = 0; // set clock to 0 again
        i++;
    } // while
    // Now clock bits from shift-registers to output-registers
    STCPb     = 1; // set clock to 1
    colmask <<= 1; // small delay
    STCPb     = 0; // set clock to 0 again
} // send_to_hc595()

/*-----------------------------------------------------------------------------
  Purpose  : This function is called every 500 msec. from adc_task() and
             calculates the output level to display on SSD 2. This output-level
             is only dependent of amplitude (VRMS, VPEAK or VPP).
  Variables: *dp   : the code for the decimal-point on SSD1
              *leds: which led to display: LED_VPP, LED_VPK, LED_VRMS.
  Returns  : the input-level to display on SSD 3.
  ---------------------------------------------------------------------------*/
uint16_t calc_output_level(uint8_t *dp, uint8_t *leds)
{
    float x = (float)read_adc(ADC1); // read value from ADC1
    
    switch (amplitude)
    {
      case VRMS:
        x        *= ADC1_FS_VRMS;
        VPK_LEDb  = 0;
        VRMS_LEDb = 1;
        *leds     = LED_VRMS;
        break;
      case VPEAK:
        x        *= ADC1_FS_VPK;
        VPK_LEDb  = 1;
        VRMS_LEDb = 0;
        *leds     = LED_VPK;
        break;
      default: // VPP
        x        *= ADC1_FS_VPP;
        VPK_LEDb  = 1; // both leds to 1 for now
        VRMS_LEDb = 1;
        *leds     = LED_VPP;
        break;
    } // switch
    if (x > 9999.5)
    {
      *dp  = DP2;   // dp for 1x.yy number format
       x  *= 0.1;   // divide by 10
    } // if
    else *dp = DP1; // dp for 1.yyy number format
    return (uint16_t)(x + 0.5);
} // calc_output_level()

/*-----------------------------------------------------------------------------
  Purpose  : This function is called every 500 msec. from adc_task() and
             calculates the input level to display on SSD 3. This input-level
             is dependent of amplitude (VRMS, VPEAK or VPP) and which input
             level is selected (100V, 30V, 10V, 3V or 1V).
  Variables: *leds: which led to display: LED_VPP, LED_VPK, LED_VRMS.
  Returns  : the input-level to display on SSD 3.
  ---------------------------------------------------------------------------*/
uint16_t calc_input_level(uint8_t *dp)
{
      float x = (float)read_adc(ADC2);
      
      switch (amplitude)
      {
         case VRMS: x *= ADC2_FS_VRMS; break;
         case VPP : x *= ADC2_FS_VPP ; break;
         default  : x *= ADC2_FS_VPK ; break; // Vpeak
      } // switch
      switch (lvl_in)
      {
          case INPUT_100V: x *= lvl_in_c100; break;
          case INPUT_30V : x *= lvl_in_c30 ; break;
          case INPUT_10V : x *= lvl_in_c10 ; break;
          case INPUT_1V  : x *= lvl_in_c1  ; break;
          default        : x *= lvl_in_c3  ; break; // pass-through
      } // switch      
    if (x > 9999.5)
    {
      *dp  = DP2;   // dp for 1x.yy number format
       x  *= 0.1;   // divide by 10
    } // if
    else *dp = DP1; // dp for 1.yyy number format
    return (uint16_t)(x + 0.5);
} // calc_input_level()

/*-----------------------------------------------------------------------------
  Purpose  : This function is called every 500 msec. from adc_task() and
             calcuates the distortion level to display on SSD 4. This level
             is dependent of dist_db (DIST_PERC, DIST_DB) and which sensitivity
             level is selected (10%, 3%, 1%, 0.3%, 0.1%, 0.03%, 0.01 and 0.003%).
  Variables: *dp: the code for the decimal-point on SSD4.
             *leds: which led to display: LED_DB, LED_PPM, LED_PERC.
  Returns  : the number to display on SSD 4
  ---------------------------------------------------------------------------*/
int16_t calc_dist_level(uint8_t *dp, uint8_t *leds)
{
      float x   = (float)read_adc(ADC3);
      bool  ppm = (sensi < SENS_0_100); // ppm for lowest distortion levels
      
      switch (sensi)
      {
          case SENS_10_000: x *= sensi_10c ; break; // 9.999 %
          case SENS_3_000 : x *= sensi_3c  ; break; // 3.000 %
          case SENS_1_000 : x *= sensi_1c  ; break; // 1.000 %
          case SENS_0_300 : x *= sensi_c3  ; break; // 0.300 %
          case SENS_0_100 : x *= sensi_c1  ; break; // 0.100 %
          case SENS_0_030 : x *= sensi_c03 ; break; // 300.0 ppm
          case SENS_0_010 : x *= sensi_c01 ; break; // 100.0 ppm
          default         : x *= sensi_c003; break; //  30.0 ppm
      } // switch      
      if (dist_db == DIST_DB)
      {   // display dB number as '-xx.y'
          *dp   = DP3;    // set decimal-point for dB numbers
          *leds = LED_DB; // light dB LED next to SSD4
          if (x > 0.0) 
               x =   200.0 * log10f(x) - (ppm ? 1400.0 : 1000.0);
          else x = -1200.0; // -120 dB
          if (x <= -1000.0) 
          {   // no decimal-point for number <= -100 dB
              x  *= 0.1;    // display dB as '-1xx'
              *dp = DP_OFF; // no decimal-point
          } // if
      } // if
      else if (ppm) { *dp = DP3; *leds = LED_PPM;  } // set dp & LED for ppm
      else          { *dp = DP1; *leds = LED_PERC; } // set dp & LED for %
      return (int16_t)x;
} // calc_dist_level()

/*-----------------------------------------------------------------------------
  Purpose  : This task is called every 500 msec. and reads the following
             analog signals:
             AIN1: Output Level of generated sine-wave
             AIN2: Input Level
             AIN3: Distortion Level
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void adc_task(void)
{
    uint8_t dots = DP1;     // dp for 1.xxx number format
    uint8_t leds = LED_OFF; // Seven-segment display is off by default
    
    //--------------------------------------------------------------------
    // Output-level display on SSD1, depends on amplitude selection. The 
    // Vpp, Vrms and Vpeak LEDs (leds) are set by calc_output_level().
    //--------------------------------------------------------------------
    lvl_out_adc = calc_output_level(&dots,&leds); 
    tm1637_set_brightness(SSD_LVL_OUT, brightness, true); // SSD brightness
    tm1637_show_nr_dec_ex(SSD_LVL_OUT, lvl_out_adc, dots, true, 4, 0, leds);

    //--------------------------------------------------------------------
    // Input-level display on SSD3, depends on both selected input-level
    // and amplitude selection. The Vpp, Vrms and Vpeak LEDs (leds) are 
    // the same as for the output-level.
    //--------------------------------------------------------------------
    lvl_in_adc  = calc_input_level(&dots); 
    tm1637_set_brightness(SSD_LVL_IN, brightness, true); // SSD brightness
    tm1637_show_nr_dec_ex(SSD_LVL_IN, lvl_in_adc, dots, true, 4, 0, leds);

    //--------------------------------------------------------------------
    // Distortion display on SSD4, depends on both selected sensitivity and
    // unity (dB, % or ppm). The ppm unity is selected for percentages < 0.1%.
    //--------------------------------------------------------------------
    lvl_dist_adc = calc_dist_level(&dots,&leds);
    tm1637_set_brightness(SSD_DIST, brightness, true); // SSD brightness
    tm1637_show_nr_dec_ex(SSD_DIST, lvl_dist_adc, dots, true, 4, 0, leds);
} // adc_task()

/*-----------------------------------------------------------------------------
  Purpose  : This routine does a divide by 10 using only shifts
  Variables: n: the number to divide by 10
  Returns  : the result
  ---------------------------------------------------------------------------*/
uint16_t divu10(uint16_t n)
{
  uint16_t q, r;

  q = (n >> 1) + (n >> 2);       // 1/2 + 1/4 = 3/4
  q = q + (q >> 4);              // 3/4 + 3/64 = 51/64
  q = q + (q >> 8);              // 51/64 + 51/(16384) = 13107/16384
  q = q >> 3;                    // 13107 / 131072
  r = n - ((q << 3) + (q << 1)); // 1 - (13107/16384 + 13107/65536) = 1/65536
  return q + ((r + 6) >> 4);     // 13107/131072 + 1/1048576 = 104857 / 1048576
} // divu10()

/*-----------------------------------------------------------------------------
  Purpose  : This function converts a frequency-index into a frequency in a
             string. The global variable freq_sine is used as input.
  Variables: s: the string to return
  Returns  : -
  ---------------------------------------------------------------------------*/
void freqkHz(char *s)
{
  strcpy(s,hz[freq_sine%10]);
  if (freq_sine < FREQ_100_HZ)
  { // 20, 25, 30, 40, 50, 65, 80 Hz
    strcat(s," Hz  ");
  } // if
  else if (freq_sine < FREQ_1000_HZ)
  { // 100, 130, 160, 200, 250, 300, 400, 500, 650, 800 Hz
    strcat(s,"0 Hz ");
  } // else if
  else if (freq_sine < FREQ_10_KHZ)
  { // 1000, 1300, 1600, 2000, 2500, 3000, 4000, 5000, 6500, 8000 Hz
    strcat(s,"00 Hz");
  } // else if
  else if (freq_sine < FREQ_100_KHZ)
  { // 10, 13, 16, 20, 25, 30, 40, 50, 65, 80 kHz
    strcat(s," kHz ");
  } // else if
  else
  { // 100, 130, 160, 200 kHz
    strcat(s,"0 kHz");
  } // else
} // freqkHz()

/*-----------------------------------------------------------------------------
  Purpose  : This task is called every 200 msec. and updates both the seven-
             segment display 1 (SSD1) and the LCD-display.
             SSD1: shows the actual frequency of the generated sine-wave, 
                   based on the measured clock-ticks of timer1.
             LCD:  shows all settings made by the user.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void freq_task(void)
{
    char     s[30];
    char     s1[10];
    uint8_t  dots;       // dots indicator for seven-segment display
    bool     khz = true; // assume value in kHz
    
    BG_LEDb = 1; // LED on
    calc_freq(); // get actual frequency in freq_meas and fmt_meas
    //------------------------------------------------------------------
    // LED-Display 1: display measured frequency of generated sine-wave
    //------------------------------------------------------------------
    {   
        switch (fmt_meas) // value from calc_freq()
        {
          case DP0_HZ:        // display value in Hz, no decimals
               khz  = false;  // value in Hz
               dots = DP_OFF; // no decimal-point 
               break;
          case DP1_KHZ:       // display value in kHz, 1 decimal
               dots = DP3;    // decimal-point for xxx.y number format
               break;
          default:            // DP2_KHZ, display value in kHz, 2 decimals
               dots = DP2;    // decimal-point for xx.yy number format
               break;
        } // switch
        HZb  = !khz; // Set frontpanel LEDs, TODO: remove if new TM1637 PCBs are used
        KHZb =  khz; // kHz LED on
        if (freq_meas != freq_meas_old)
        {   // Only use TM1637 when value is changed
            tm1637_set_brightness(SSD_FREQ,brightness,true); // brightness SSD1, actual frequency
            tm1637_show_nr_dec_ex(SSD_FREQ,freq_meas,dots,false,4,0,khz ? LED_KHZ : LED_HZ); // no LZ, 4 digits
            freq_meas_old = freq_meas;
        } // if
    } // if

    //------------------------------------------------------------------
    // LCD-Display: update all settings in case of a change
    //------------------------------------------------------------------
    if (freq_sine != freq_old)
    {   // Second line of LCD-Display
        lcd_i2c_setCursor(0,1);
        lcd_i2c_print("FREQ:");
        lcd_i2c_setCursor(5,1);
        freqkHz(s); // convert freq_sine to a string
        sprintf(s1," RANGE:%d",range);
        strcat(s,s1);
        lcd_i2c_print(s);        // print string to LCD-display
        setup_timer1(freq_sine); // Setup Timer1 frequency/period measurement
        freq_old = freq_sine;    // update previous value of freq_sine
    } // if
    else if ((lvl_out != lo_old) || (lvl_in != li_old))
    {   // Third line of LCD-Display
        lcd_i2c_setCursor(0,2);
        strcpy(s,"LVL-OUT:");
        switch (lvl_out)
        {
            case LEVEL_0V15: strcat(s,".15V"); break;
            case LEVEL_0V50: strcat(s,"0.5V"); break;
            case LEVEL_1V50: strcat(s,"1.5V"); break;
            case LEVEL_5V  : strcat(s,"5.0V"); break;
            default        : strcat(s," OFF"); break;
        } // switch
        strcat(s," IN:");
        switch (lvl_in)
        {
            case INPUT_3V  : strcat(s,"3V  "); break;
            case INPUT_10V : strcat(s,"10V "); break;
            case INPUT_30V : strcat(s,"30V "); break;
            case INPUT_100V: strcat(s,"100V"); break;
            default        : strcat(s,"1V  "); break;
        } // switch
        lcd_i2c_print(s);
        if ((lo_old == LEVEL_OFF) && (lvl_out != LEVEL_OFF))
        {   // Freq. measurement was disabled at LEVEL_OFF, so enable it again
            setup_timer1(freq_sine);
        } // if
        lo_old = lvl_out;
        li_old = lvl_in;
    } // if
    else if (sensi != sensi_old)
    {   // Bottom line of LCD-display
        lcd_i2c_setCursor(0,3);
        strcpy(s,"SENS:");
        switch (sensi)
        {
            case SENS_0_003: strcat(s,"30ppm "); break;
            case SENS_0_010: strcat(s,"100ppm"); break;
            case SENS_0_030: strcat(s,"300ppm"); break;
            case SENS_0_100: strcat(s,"0.1%  "); break;
            case SENS_0_300: strcat(s,"0.3%  "); break;
            case SENS_1_000: strcat(s,"1.0%  "); break;
            case SENS_3_000: strcat(s,"3.0%  "); break;
            default        : strcat(s,"10.0% "); break;
        } // switch
        lcd_i2c_print(s);
        sensi_old = sensi;
    } // if
    BG_LEDb = 0; // LED off
} // freq_task()

/*-----------------------------------------------------------------------------
  Purpose  : This routine reads the push-buttons UP, DOWN, LEFT, RIGHT, OK
             FP10, FM10, PPPKRMS and DBPERC that are connected to connector J2
             of the TM1637 SSD PCB.
             The global variables buttons_aux1 and buttons_aux2 are updated.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void read_aux_buttons(void)
{
    uint8_t tm1637_keys;
    uint8_t x1 = 0;
    uint8_t x2 = 0;
    
    tm1637_keys = tm1637_read_keys(SSD_FREQ);
    if (tm1637_keys <= TM1637_KEY_OK)
    {   // UP:0, DOWN:1, LEFT:2, RIGHT:3, OK:4
        // Convert to buttons lay-out: Bit 4..0: UP, DOWN, LEFT, RIGHT, OK
        x1 = (1 << (4-tm1637_keys));
    } // else if
    else if (tm1637_keys <= TM1637_KEY_DBPERC)
    {   // FP10:8, FM10:9, PPPKRMS:10 and DBPERC:11 values
        x2 = (1 << (11-tm1637_keys)); // new values now in bits 3..0
    } // else
    // else: do nothing, tm1637_keys == 0xFF, no key pressed
    buttons_aux1 <<= 8; // Shift older values of push-buttons
    buttons_aux1 |= x1; // New value of push-buttons now in bits 4..0
    buttons_aux2 <<= 8; // Shift older values of push-buttons
    buttons_aux2 |= x2; // New value of push-buttons now in bits 3..0
} // read_aux_buttons()

/*-----------------------------------------------------------------------------
  Purpose  : This task is called every 100 msec. and contains reading of the
             push-buttons and taking actions on it.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void ctrl_task(void)
{
    uint8_t f  = freq_sine;
    uint8_t lo = lvl_out;
    uint8_t li = lvl_in;
    uint8_t s  = sensi;
    char    str[10];
    
    lcd_i2c_setCursor(0,0);  // Start of first row
    read_buttons();          // Nav. buttons connected to uC PCB
    read_aux_buttons();      // Keys connected to TM1637 PCB connector J2
    if (!BTN_IDLE(BTN_ANY)) std_tmr = TMR_NO_KEY;
    else if (std_tmr)       std_tmr--; // countdown counter

    switch (menustate)
    {
        case STD_IDLE:
            lcd_i2c_print(version);
            if (BTN_PRESSED(BTN_UP))
            {
                    if (f < FREQ_200_KHZ) set_frequency(f+1,SEND);
            } // if
            else if (BTN_PRESSED(BTN_DOWN))
            {
                    if (f > FREQ_20_HZ) set_frequency(f-1,SEND);
            } // else if
            else if (BTN_PRESSED(BTN_RIGHT))
            {
                    menustate = STD_LVL_OUT;
            } // else if
            else if (BTN_PRESSED(BTN_LEFT))
            {
                    menustate = STD_SWEEP;  // goto sweep-mode 
                    sweep_tmr = TMR_SWEEP;  // init. sweep timer
                    freq_save = freq_sine;  // save current value of freq_sine
                    fsweep    = FREQ_20_HZ; // sweep start-frequency is 20 Hz
                    set_frequency(fsweep,SEND);
            } // else if
            else if (BTN_PRESSED(BTN_OK))
            {
                menustate = STD_AMPL;
            } // else if
            else if (AUX_PRESSED(BTN_FP10))
            {
                    if (f < FREQ_200_KHZ-9) set_frequency(f+10,SEND);
            } // else if
            else if (AUX_PRESSED(BTN_FM10))
            {
                    if (f > FREQ_20_HZ+9) set_frequency(f-10,SEND);
            } // else if
            else if (AUX_PRESSED(BTN_PPPKRMS))
            {
                if (++amplitude > VPP) amplitude = VRMS; 
                eeprom_write_byte(AMPL_UNITY_ADDR,amplitude);
            } // else if
            else if (AUX_PRESSED(BTN_DBPERC))
            {
                dist_db = !dist_db;
                eeprom_write_byte(DIST_UNITY_ADDR,(uint8_t)dist_db);
            } // else if
            break;
    case STD_LVL_OUT:
            lcd_i2c_print("SET OUTPUT-LEVEL:");
            if      (BTN_PRESSED(BTN_RIGHT))            menustate = STD_LVL_IN;
            else if (BTN_PRESSED(BTN_LEFT) || 
                     !std_tmr || BTN_PRESSED(BTN_OK))   menustate = STD_IDLE;
            else if (BTN_PRESSED(BTN_UP))
            {
                if (lo < LEVEL_5V) set_output_level(lo+1,SEND);
            } // else if
            else if (BTN_PRESSED(BTN_DOWN))
            {
                if (lo > LEVEL_OFF) set_output_level(lo-1,SEND);
            } // else if
            break;
    case STD_LVL_IN:
            lcd_i2c_print("SET INPUT-LEVEL  ");
            if      (BTN_PRESSED(BTN_RIGHT)) menustate = STD_SENS;
            else if (BTN_PRESSED(BTN_LEFT))  menustate = STD_LVL_OUT;
            else if (BTN_PRESSED(BTN_UP))
            {
                if (li < INPUT_100V) set_input_level(li+1,SEND);
            } // else if
            else if (BTN_PRESSED(BTN_DOWN))
            {
                if (li > INPUT_1V) set_input_level(li-1,SEND);
            } // else if
            else if (!std_tmr || BTN_PRESSED(BTN_OK)) menustate = STD_IDLE;
            break;
    case STD_SENS:
            lcd_i2c_print("SET SENSITIVITY: ");
            if (BTN_PRESSED(BTN_RIGHT))
            {
                menustate = STD_SWEEP;  // goto sweep-mode
                sweep_tmr = TMR_SWEEP;  // init. sweep timer
                freq_save = freq_sine;  // save current value of freq_sine
                fsweep    = FREQ_20_HZ; // sweep start-frequency is 20 Hz
                set_frequency(fsweep,SEND);
            } // if
            else if (BTN_PRESSED(BTN_LEFT))  menustate = STD_LVL_IN;
            else if (BTN_PRESSED(BTN_UP))
            {
                if (s < SENS_10_000) set_sensitivity(s+1,SEND);
            } // else if
            else if (BTN_PRESSED(BTN_DOWN))
            {
                if (s > SENS_0_003) set_sensitivity(s-1,SEND);
            } // else if
            else if (!std_tmr || BTN_PRESSED(BTN_OK)) menustate = STD_IDLE;
            break;
    case STD_SWEEP:
            lcd_i2c_print("SWEEP FREQUENCIES");
            std_tmr = TMR_NO_KEY; // prevent menu time-out
            if (BTN_PRESSED(BTN_RIGHT))
            {
                freq_sine = freq_save; // restore frequency before sweep
                menustate = STD_IDLE;  
            } // if
            else if (BTN_PRESSED(BTN_LEFT))
            {
                freq_sine = freq_save; // restore frequency before sweep
                menustate = STD_SENS;
            } // else if
            else if (--sweep_tmr == 0)
            {
                sweep_tmr = TMR_SWEEP;
                if (fsweep < FREQ_200_KHZ)
                     set_frequency(++fsweep,SEND); // next frequency
                else
                {
                    menustate = STD_IDLE;
                    set_frequency(FREQ_1000_HZ,SEND); // back to default value
                } // else
            } // if
            break;
    case STD_AMPL:
            lcd_i2c_print("AMPLITUDE: ");
            if (BTN_PRESSED(BTN_RIGHT))
            {
                menustate = STD_DIST;
            } // else if
            else if (BTN_PRESSED(BTN_LEFT))
            {
                menustate = STD_BRIGHT;
            } // else if
            else if (BTN_PRESSED(BTN_UP))
            {
                if (++amplitude > VPP) amplitude = VRMS;
            } // else if
            else if (BTN_PRESSED(BTN_DOWN))
            {
                if (--amplitude < VRMS) amplitude = VPP;
            } // else if
            else if (!std_tmr || BTN_PRESSED(BTN_OK)) 
            {   // exit and write to eeprom
                eeprom_write_byte(AMPL_UNITY_ADDR,amplitude);
                menustate = STD_IDLE;
            } // else if
            if (amplitude == VRMS)
                 lcd_i2c_print("RMS   ");
            else if (amplitude == VPEAK)
                 lcd_i2c_print("PEAK  ");
            else lcd_i2c_print("PK-PK ");
            break;
    case STD_DIST:
            lcd_i2c_print("DISTORTION: ");
            if (BTN_PRESSED(BTN_RIGHT))
            {
                menustate = STD_BRIGHT;
            } // else if
            else if (BTN_PRESSED(BTN_LEFT))
            {
                menustate = STD_AMPL;
            } // else if
            else if (BTN_PRESSED(BTN_UP) || BTN_PRESSED(BTN_DOWN))
            {
                dist_db = !dist_db;
            } // else if
            else if (!std_tmr || BTN_PRESSED(BTN_OK)) 
            {   // exit and write to eeprom
                eeprom_write_byte(DIST_UNITY_ADDR,(uint8_t)dist_db);
                menustate = STD_IDLE;
            } // else if
            if (dist_db)
                 lcd_i2c_print("dB   ");
            else lcd_i2c_print("%/ppm");
            break;
    case STD_BRIGHT:
            lcd_i2c_print("BRIGHTNESS: ");
            if (BTN_PRESSED(BTN_RIGHT))
            {
                menustate = STD_AMPL;
            } // else if
            else if (BTN_PRESSED(BTN_LEFT))
            {
              menustate = STD_DIST;
            } // else if
            else if (BTN_PRESSED(BTN_UP))
            {
                if (++brightness > 7) brightness = 1;
            } // else if
            else if (BTN_PRESSED(BTN_DOWN))
            {
                if (--brightness < 1) brightness = 7;
            } // else if
            else if (!std_tmr || BTN_PRESSED(BTN_OK)) 
            {   // exit and write to eeprom
                eeprom_write_byte(BRIGHTNESS_ADDR,brightness);
                menustate = STD_IDLE;
            } // else if
            sprintf(str,"%d    ",brightness);
            lcd_i2c_print(str);
            break;
    } // switch
} // ctrl_task()

/*-----------------------------------------------------------------------------
  Purpose  : This function initializes the eeprom and reads all values from it.
  Variables: init: true = fill eeprom with all default values
  Returns  : -
  ---------------------------------------------------------------------------*/
void eep_init(bool init)
{
   if (init)
   {
       xputs("eeprom init ");
       if (eeprom_read_float(FS_LVL_IN_100V_ADDR) < 0.01)
       {
           eeprom_write_float(FS_LVL_IN_100V_ADDR,FS_LVL_IN_100V_VAL);
           eeprom_write_float(FS_LVL_IN_30V_ADDR ,FS_LVL_IN_30V_VAL);
           eeprom_write_float(FS_LVL_IN_10V_ADDR ,FS_LVL_IN_10V_VAL);
           eeprom_write_float(FS_LVL_IN_3V_ADDR  ,FS_LVL_IN_3V_VAL);
           eeprom_write_float(FS_LVL_IN_1V_ADDR  ,FS_LVL_IN_1V_VAL);
           
           eeprom_write_byte(AMPL_UNITY_ADDR, VPEAK);
           eeprom_write_byte(DIST_UNITY_ADDR, (uint8_t)DIST_PERC);
           eeprom_write_byte(BRIGHTNESS_ADDR, 1);
           
           eeprom_write_float(FS_SENS_10_000_ADDR,FS_SENS_10_000_VAL);
           eeprom_write_float(FS_SENS_3_000_ADDR ,FS_SENS_3_000_VAL);
           eeprom_write_float(FS_SENS_1_000_ADDR ,FS_SENS_1_000_VAL);
           eeprom_write_float(FS_SENS_0_300_ADDR ,FS_SENS_0_300_VAL);
           eeprom_write_float(FS_SENS_0_100_ADDR ,FS_SENS_0_100_VAL);
           eeprom_write_float(FS_SENS_0_030_ADDR ,FS_SENS_0_030_VAL);
           eeprom_write_float(FS_SENS_0_010_ADDR ,FS_SENS_0_010_VAL);
           eeprom_write_float(FS_SENS_0_003_ADDR ,FS_SENS_0_003_VAL);
           xputs("with default values\n");
       } // if
       else xputs("done\n"); 
   } // if
   lvl_in_c100 = eeprom_read_float(FS_LVL_IN_100V_ADDR);
   lvl_in_c30  = eeprom_read_float(FS_LVL_IN_30V_ADDR);
   lvl_in_c10  = eeprom_read_float(FS_LVL_IN_10V_ADDR);
   lvl_in_c3   = eeprom_read_float(FS_LVL_IN_3V_ADDR);
   lvl_in_c1   = eeprom_read_float(FS_LVL_IN_1V_ADDR);   
   
   amplitude   = eeprom_read_byte(AMPL_UNITY_ADDR);
   dist_db     = ((bool)eeprom_read_byte(DIST_UNITY_ADDR) == DIST_DB);
   brightness  = eeprom_read_byte(BRIGHTNESS_ADDR);
   
   sensi_10c   = eeprom_read_float(FS_SENS_10_000_ADDR);
   sensi_3c    = eeprom_read_float(FS_SENS_3_000_ADDR);
   sensi_1c    = eeprom_read_float(FS_SENS_1_000_ADDR);
   sensi_c3    = eeprom_read_float(FS_SENS_0_300_ADDR);
   sensi_c1    = eeprom_read_float(FS_SENS_0_100_ADDR);
   sensi_c03   = eeprom_read_float(FS_SENS_0_030_ADDR);
   sensi_c01   = eeprom_read_float(FS_SENS_0_010_ADDR);
   sensi_c003  = eeprom_read_float(FS_SENS_0_003_ADDR);
} // eep_init()

/*-----------------------------------------------------------------------------
  Purpose  : This is the main entry-point for the entire program.
             It initialises everything, starts the scheduler and
             dispatches tasks.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
int main(void)
{
    char    s[30];  // Needed for xputs()

    __disable_interrupt();
    initialise_system_clock();    // Use internal system-clock (16 MHz)
    uart_init();                  // UART1 init. to 57600,8,N,1
    setup_timer2(FREQ_1KHZ);      // Set Timer 2 for 1 kHz interrupt frequency
    setup_gpio_ports();           // Init. needed output-ports
    i2c_init_bb();                // Init. I2C bus for bit-banging

    // Initialise all tasks for the scheduler
    scheduler_init();                    // clear task_list struct
    add_task(adc_task ,"ADC",  0, 500);  // every 500 msec.
    add_task(freq_task,"FRQ", 50, 200);  // every 200 msec.
    add_task(ctrl_task,"CTL",200, 100);  // every 100 msec.
    __enable_interrupt();
    lcd_i2c_init(0x4E,20,4,LCD_5x8DOTS); // Needs working interrupts!
    lcd_i2c_clear();
    lcd_i2c_setCursor(0,0);
    lcd_i2c_backlight_on();

    xputs(version); // print version number
    xputs("\n");
    eep_init(true); // init eeprom values
    
    set_frequency(FREQ_1000_HZ,  NOSEND); // starting frequency
    set_output_level(LEVEL_1V50, NOSEND); // 1.5V output-level
    set_input_level(INPUT_100V,  NOSEND); // 100V input-level
    set_sensitivity(SENS_10_000, SEND);   // 10% sensitivity input

    setup_timer1(FREQ_1000_HZ);           // Init. frequency measurement
    
    while (1)
    {   // background-processes
        dispatch_tasks();     // Run task-scheduler()
        switch (rs232_command_handler()) // run command handler continuously
        {
            case ERR_CMD: xputs("Cmd Err\n"); break;
            case ERR_NUM: sprintf(s,"Num Err (%s)\n",rs232_inbuf);
                          xputs(s);
                          break;
            default     : break;
        } // switch
    } // while
} // main()
