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
char version[]          = "THD-Control V0.21";
const char hz[10][3]    = {"20","25","30","40","50","65","80","10","13","16"};

int16_t  lvl_out_adc;   // Sine-wave output level, ADC1
int16_t  lvl_in_adc;    // Input-level, ADC2
int16_t  lvl_dist_adc;  // Distortion level, ADC3
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

extern char     rs232_inbuf[];
extern uint16_t buttons;        // Previous and Actual value of press-buttons

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
    uint8_t dots = 0x80;
    
    lvl_out_adc = read_adc(ADC1); // read value from ADC1
    if (amplitude == VRMS)
    {
        lvl_out_adc = (uint16_t)(ADC1_FS_VRMS * lvl_out_adc + 0.5);
        VPK_LEDb    = 0;
        VRMS_LEDb   = 1;
    } // if
    else if (amplitude == VPEAK)
    {
        lvl_out_adc = (uint16_t)(ADC1_FS_VPK * lvl_out_adc + 0.5);
        VPK_LEDb    = 1;
        VRMS_LEDb   = 0;
    } // else if
    else
    {   // VPP
        lvl_out_adc = (uint16_t)(ADC1_FS_VPP * lvl_out_adc + 0.5);
        VPK_LEDb  = 0; // both leds to 0 for now
        VRMS_LEDb = 0;
    } // else
    if (lvl_out_adc > 9999)
    {
      dots        = 0x40;
      lvl_out_adc = divu10(lvl_out_adc);
    } // if
    tm1637_set_brightness(SSD_LVL_OUT, 1, true); // SSD brightness
    tm1637_show_nr_dec_ex(SSD_LVL_OUT, lvl_out_adc, dots, true, 4, 0);
    //--------------------------------------------------------------------
    // Both lvl_in_adc (ADC2,SSD_LVL_IN) and lvl_dist_adc (ADC3,SSD_DIST)
    // are being displayed in mV for debugging purposes.
    // Conversion: max. input level is 5000 mV (VREF), ADC-value = 1023 max.
    //             If we use 1024 as max. value, then we get 5000/1024 = 625/128.
    //             This is approx. equal to 39/8, error = 0.26 %. 
    // TODO: create proper readings on both seven-segment displays.
    //--------------------------------------------------------------------
    lvl_in_adc   = read_adc(ADC2);
    lvl_in_adc   = (int16_t)(((uint16_t)lvl_in_adc * 39 + 4)>>3); // 39/8 is approx. 5000/1023
    tm1637_set_brightness(SSD_LVL_IN, 1, true);  // SSD brightness
    tm1637_show_nr_dec_ex(SSD_LVL_IN, lvl_in_adc, 0x80, true, 4, 0);
    
    lvl_dist_adc = read_adc(ADC3);
    lvl_dist_adc = (int16_t)(((uint16_t)lvl_dist_adc * 39 + 4)>>3); // 39/8 is approx. 5000/1023
    tm1637_set_brightness(SSD_DIST, 1, true);    // SSD brightness
    tm1637_show_nr_dec_ex(SSD_DIST, lvl_dist_adc, 0x80, true, 4, 0);
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
    uint8_t  dots; // dots indicator for seven-segment display

    BG_LEDb = 1; // LED on
    calc_freq(); // get actual frequency in freq_meas and fmt_meas
    //------------------------------------------------------------------
    // LED-Display 1: display measured frequency of generated sine-wave
    //------------------------------------------------------------------
    //if (freq_meas > 0)
    {   // only update display when a valid measurement is made
        switch (fmt_meas) // value from calc_freq()
        {
          case DP0_HZ:  // display value in Hz, no decimals
               HZb = 1; KHZb = 0; dots = 0x00; 
               break;
          case DP1_KHZ: // display value in kHz, 1 decimal
               HZb = 0; KHZb = 1; dots = 0x20; 
               break;
          default: // DP2_KHZ, display value in kHz, 2 decimals
               HZb = 0; KHZb = 1; dots = 0x40;
        } // switch
        if (freq_meas != freq_meas_old)
        {   // Only use TM1637 when value is changed
            tm1637_set_brightness(SSD_FREQ,1,true); // brightness SSD1, actual frequency
            tm1637_show_nr_dec_ex(SSD_FREQ,freq_meas,dots,false,4,0); // no LZ, 4 digits
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
        lo_old = lvl_out;
        li_old = lvl_in;
    } // if
    else if (sensi != sensi_old)
    {   // Bottom line of LCD-display
        lcd_i2c_setCursor(0,3);
        strcpy(s,"SENS:");
        switch (sensi)
        {
            case SENS_0_003: strcat(s,"0.003%"); break;
            case SENS_0_010: strcat(s,"0.01% "); break;
            case SENS_0_030: strcat(s,"0.03% "); break;
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
} // std_task()

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

    lcd_i2c_setCursor(0,0);  // Start of first row
    read_buttons();
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
            break;
    case STD_LVL_OUT:
            lcd_i2c_print("SET OUTPUT-LEVEL:");
            if      (BTN_PRESSED(BTN_RIGHT))            menustate = STD_LVL_IN;
            else if (BTN_PRESSED(BTN_LEFT) || !std_tmr) menustate = STD_IDLE;
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
            else if (!std_tmr) menustate = STD_IDLE;
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
            else if (!std_tmr) menustate = STD_IDLE;
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
            if (BTN_PRESSED(BTN_UP))
            {
                if (++amplitude > VPP) amplitude = VRMS;
            } // else if
            else if (BTN_PRESSED(BTN_DOWN))
            {
                if (--amplitude < VRMS) amplitude = VPP;
            } // else if
            else if (!std_tmr || BTN_PRESSED(BTN_OK)) menustate = STD_IDLE;
            if (amplitude == VRMS)
                 lcd_i2c_print("RMS   ");
            else if (amplitude == VPEAK)
                 lcd_i2c_print("PEAK  ");
            else lcd_i2c_print("PK-PK ");
            break;
    } // switch
} // ctrl_task()

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
