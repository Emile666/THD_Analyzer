/*==================================================================
    File Name    : tm1637.c
    Author       : Emile
    ------------------------------------------------------------------
    Purpose : This files contains the routines for the TM1637 clock ICs.
    It is converted from the original C++ Arduino library to
    a C version for the STM8.
    Original author: avishorp@gmail.com (Arduino library)
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
    along with this file. If not, see <http://www.gnu.org/licenses/>.
==================================================================*/ 
#include "tm1637.h"
#include "delay.h"

//-----------------------------------
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
//-----------------------------------
const uint8_t digit_to_segment[16] = 
{
    //   XGFEDCBA
    0x3F, // 0b00111111,    // 0
    0x06, // 0b00000110,    // 1
    0x5B, // 0b01011011,    // 2
    0x4F, // 0b01001111,    // 3
    0x66, // 0b01100110,    // 4
    0x6D, // 0b01101101,    // 5
    0x7D, // 0b01111101,    // 6
    0x07, // 0b00000111,    // 7
    0x7F, // 0b01111111,    // 8
    0x6F, // 0b01101111,    // 9
    0x77, // 0b01110111,    // A
    0x7C, // 0b01111100,    // b
    0x39, // 0b00111001,    // C
    0x5E, // 0b01011110,    // d
    0x79, // 0b01111001,    // E
    0x71  // 0b01110001     // F
};

const uint16_t divisors[] = { 1, 10, 100, 1000 };

/*-----------------------------------------------------------------------------
Purpose  : This function sets the data-pin of the selected TM1637 to input.
            This is used to read the ACK-bit of the IC.
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_dio_pin_input(uint8_t ssd_nr)
{
    switch (ssd_nr)
    {
        case SSD_FREQ:
            PG_DDR &= ~DIO1; // Set as input
            PG_CR1 |=  DIO1; // Enable Pull-up resistor	
            break;
        case SSD_LVL_OUT:
            PD_DDR &= ~DIO2; // Set as input
            PD_CR1 |=  DIO2; // Enable Pull-up resistor	
            break;
        case SSD_LVL_IN:
            PD_DDR &= ~DIO3; // Set as input
            PD_CR1 |=  DIO3; // Enable Pull-up resistor	
            break;
        default: // SSD_DIST
            PE_DDR &= ~DIO4; // Set as input
            PE_CR1 |=  DIO4; // Enable Pull-up resistor	
            break;
    } // switch
} // tm1637_dio_pin_input()

/*-----------------------------------------------------------------------------
Purpose  : This function sets the data-pin of the selected TM1637 to output.
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_dio_pin_output(uint8_t ssd_nr)
{
    switch (ssd_nr)
    {
        case SSD_FREQ:
            PG_DDR |=  DIO1; // Set as output
            PG_ODR &= ~DIO1; // Set dio_pin low	
            break;
        case SSD_LVL_OUT:
            PD_DDR |=  DIO2; // Set as output
            PD_ODR &= ~DIO2; // Set dio_pin low	
            break;
        case SSD_LVL_IN:
            PD_DDR |=  DIO3; // Set as output
            PD_ODR &= ~DIO3; // Set dio_pin low	
            break;
        default: // SSD_DIST
            PE_DDR |=  DIO4; // Set as output
            PE_ODR &= ~DIO4; // Set dio_pin low	
            break;
    } // switch
} // tm1637_dio_pin_output()

/*-----------------------------------------------------------------------------
Purpose  : This function sets the clock-pin of the selected TM1637 to low.
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_clk0(uint8_t ssd_nr)
{
    switch (ssd_nr)
    {   // Set clocks to 0
        case SSD_FREQ   : CLK1b = 0; break; 
        case SSD_LVL_OUT: CLK2b = 0; break;
        case SSD_LVL_IN : CLK3b = 0; break;
        default         : CLK4b = 0; break; // SSD_DIST
    } // switch
    delay_usec(TM1637_DELAY_USEC);
} // tm1637_clk0()

/*-----------------------------------------------------------------------------
Purpose  : This function sets the clock-pin of the selected TM1637 to high.
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_clk1(uint8_t ssd_nr)
{
    switch (ssd_nr)
    {   // Set clocks to 1
        case SSD_FREQ   : CLK1b = 1; break;
        case SSD_LVL_OUT: CLK2b = 1; break;
        case SSD_LVL_IN : CLK3b = 1; break;
        default         : CLK4b = 1; break; // SSD_DIST
    } // switch
    delay_usec(TM1637_DELAY_USEC);
} // tm1637_clk1()

/*-----------------------------------------------------------------------------
Purpose  : This function sets the data-pin of the selected TM1637 to low.
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_dio0(uint8_t ssd_nr)
{
    switch (ssd_nr)
    {   // set data to 0
        case SSD_FREQ   : DIO1b = 0; break;
        case SSD_LVL_OUT: DIO2b = 0; break;
        case SSD_LVL_IN : DIO3b = 0; break;
        default         : DIO4b = 0; break; // SSD_DIST
    } // switch
    delay_usec(TM1637_DELAY_USEC);
} // tm1637_dio0()

/*-----------------------------------------------------------------------------
Purpose  : This function sets the data-pin of the selected TM1637 to high.
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_dio1(uint8_t ssd_nr)
{
    switch (ssd_nr)
    {   // Set data to 1
        case SSD_FREQ   : DIO1b = 1; break;
        case SSD_LVL_OUT: DIO2b = 1; break;
        case SSD_LVL_IN : DIO3b = 1; break;
        default         : DIO4b = 1; break; // SSD_DIST
    } // switch
    delay_usec(TM1637_DELAY_USEC);
} // tm1637_dio1()

/*-----------------------------------------------------------------------------
Purpose  : This function reads the data-pin of the selected TM1637.
Variables: ssd_nr: the SSD number
Returns  : [ack, nak]
---------------------------------------------------------------------------*/
bool tm1637_dio_read(uint8_t ssd_nr)
{
    bool    ack;

    switch (ssd_nr)
    {
        case SSD_FREQ   : ack = ((PG_IDR & DIO1) == DIO1); break;
        case SSD_LVL_OUT: ack = ((PD_IDR & DIO2) == DIO2); break;
        case SSD_LVL_IN : ack = ((PD_IDR & DIO3) == DIO3); break;
        default         : ack = ((PE_IDR & DIO4) == DIO4); break;
    } // switch
    return ack;
} // tm1637_dio_read()

/*-----------------------------------------------------------------------------
Purpose  : This function writes a start sequence to the TM1637
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_start(uint8_t ssd_nr)
{
    switch (ssd_nr)
    {   // just to make sure
        case SSD_FREQ   : PG_ODR |=  (DIO1 | CLK1); break;
        case SSD_LVL_OUT: PD_ODR |=  (DIO2 | CLK2); break;
        case SSD_LVL_IN : PD_ODR |=  (DIO3 | CLK3); break;
        default         : PE_ODR |=  (DIO4 | CLK4); break;
    } // switch
    tm1637_dio0(ssd_nr); // Generate Start condition
    delay_usec(TM1637_DELAY_USEC);
} // tm1637_start()

/*-----------------------------------------------------------------------------
Purpose  : This function writes a stop sequence to the TM1637
Variables: ssd_nr: the SSD number
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_stop(uint8_t ssd_nr)
{
    tm1637_dio0(ssd_nr); // Make sure that dio pin is low
    tm1637_clk1(ssd_nr); // Reset clock pin
    tm1637_dio1(ssd_nr); // Generate Stop condition
} // tm1637_stop()

/*-----------------------------------------------------------------------------
Purpose  : This function controls the brightness of the selected TM1637.
Variables: ssd_nr    : the SSD number 
           brightness: [0..7], brightness from 1/16 to 14/16
           on        : [false,true] display is OFF (false) or ON (true)
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_set_brightness(uint8_t ssd_nr, uint8_t brightness, bool on)
{
    uint8_t m_brightness = 0;
    
    m_brightness = (brightness & 0x07) | (on? 0x08 : 0x00);
    // Write COMM3 + brightness
    tm1637_start(ssd_nr);
    tm1637_write_byte(ssd_nr, TM1637_I2C_COMM3 + (m_brightness & 0x0f));
    tm1637_stop(ssd_nr);
} // tm1637_set_brightness()

/*-----------------------------------------------------------------------------
Purpose  : This function sets the various segments of the TM1637.
Variables: ssd_nr    : the SSD number
           segments: pointer to buffer with segment values
           length  : number of segment to write to
           pos     : [0..7] address of 1st segment to write to [C0H..C5H]
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_set_segments(uint8_t ssd_nr, uint8_t *segments, uint8_t length, uint8_t pos)
{
    uint8_t k;
    
    // Write COMM1
    tm1637_start(ssd_nr);
    tm1637_write_byte(ssd_nr, TM1637_I2C_COMM1); // data write mode setting
    tm1637_stop(ssd_nr);
    
    // Write COMM2 + first digit address
    tm1637_start(ssd_nr);
    tm1637_write_byte(ssd_nr, TM1637_I2C_COMM2 + (pos & 0x03));
    
    // Write the data bytes
    for (k = 0; k < length; k++) 
        tm1637_write_byte(ssd_nr, segments[k]);
    tm1637_stop(ssd_nr);
} // tm1637_set_segments()

/*-----------------------------------------------------------------------------
Purpose  : This function writes a number into a BCD value for the display.
           It writes just a number, dots are not lighted.
Variables: ssd_nr : the SSD number
           num    : the number to write to the TM1637
           lzero  : true = add leading-zeros to number on display
           length : [1..4], the number of numbers to write to the display
           pos    : [0..3], the first segment to write to
 Returns  : -
---------------------------------------------------------------------------*/
void tm1637_show_nr_dec(uint8_t ssd_nr, int16_t num, bool leading_zero, uint8_t length, uint8_t pos)
{
    tm1637_show_nr_dec_ex(ssd_nr, num, 0, leading_zero, length, pos, 0);
} // tm1637_show_nr_dec()

/*-----------------------------------------------------------------------------
Purpose  : This function converts a number into a BCD value for the display.
           It also displays one or more dots on the display.
Variables: ssd_nr : the SSD number
           num    : the number to write to the TM1637
           dots   : bits 7..4 control the dots, bit 7 is the center-colon (:)
           lzero  : true = add leading-zeros to number on display
           length : [1..4], the number of digits to write to the display
           pos    : [0..3], the position of the least significant digit
           leds   : [0..7], set the individual leds for Vpp, Vrms, Vp, dB etc.
Returns  : -
---------------------------------------------------------------------------*/
void tm1637_show_nr_dec_ex(uint8_t ssd_nr, int16_t num, uint8_t dots, bool lzero,
                           uint8_t length, uint8_t pos, uint8_t leds)
{
    uint8_t          digit, k, digits[5];
    int16_t          d, divisor;
    bool             leading = true;
    
    if (num < 0)
    { // negative number
      k         = 1;
      num       = -num;
      digits[0] = DISP_MIN;
    } // if
    else k = 0;
    while (k < 4)
    {
        divisor = divisors[3-k];
        d       = num / divisor;
        
        if (d == 0) 
        {
            if (lzero || !leading || (k == 3))
                 digit = digit_to_segment[d & 0x0F];
            else digit = DISP_OFF;
        } // if
        else 
        {
            digit   = digit_to_segment[d & 0x0F];
            num    -= d * divisor;
            leading = false;
        } // else
        
        // Add the decimal point/colon to the digit
        if (dots & (1<<(7-k)))
        {   // set decimal-point for this digit
            digit |= DP_ON; 
        } // if
        digits[k++]  = digit;
    } // while
    digits[4] = leds; // set the individual leds
    tm1637_set_segments(ssd_nr, digits + (4 - length), length+1, pos);
} // tm1637_show_nr_dec_ex()

/*-----------------------------------------------------------------------------
Purpose  : This function writes a byte to the TM1637
Variables: ssd_nr : the SSD number
           b      : the byte to write to the TM1637 
Returns  : [ack, nak]
---------------------------------------------------------------------------*/
bool tm1637_write_byte(uint8_t ssd_nr, uint8_t b)
{
    bool    ack;
    uint8_t i;
    uint8_t data = b;
    
    tm1637_clk0(ssd_nr); // Set clock to 0
    for (i = 0; i < 8; i++) 
    {   // 8 Data Bits
        // Set data bit (start with LSB)
        if (data & 0x01)
             tm1637_dio1(ssd_nr);
        else tm1637_dio0(ssd_nr);
            
        tm1637_clk1(ssd_nr);  // Now set clock high
        tm1637_clk0(ssd_nr);  // Make sure that clock is low
        data = data >> 1;     // next data-bit
    } // for
    
    // Wait for ack-bit from TM1637
    tm1637_dio_pin_input(ssd_nr);  // clk_pin now input with pull-up resistor
    delay_usec(TM1637_DELAY_USEC);
    tm1637_clk1(ssd_nr);  // Now set clock high
    ack = tm1637_dio_read(ssd_nr);
    delay_usec(TM1637_DELAY_USEC);
    tm1637_clk0(ssd_nr); // Now set clock low again
    tm1637_dio_pin_output(ssd_nr); // dio_pin is set to output and low
    return ack;
} // tm1637_write_byte()
