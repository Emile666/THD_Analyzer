/*==================================================================
  File Name    : i2c_bb.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the I2C related functions for the STM8 uC.
  ------------------------------------------------------------------
  This is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  It is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this file.  If not, see <http://www.gnu.org/licenses/>.
  ==================================================================*/ 
#include "i2c_bb.h"

/*-----------------------------------------------------------------------------
  Purpose  : This function creates a 5 usec delay (approximately) without
             using a timer or an interrupt.
             Works for fclk = 16 MHz, verified with Logic-Analyzer.
             i=40: 11.67 us, i=30: 9.17 us, i=25: 7.9 us, i=20: 6.67 us
  Variables: --
  Returns  : -
  ---------------------------------------------------------------------------*/
void i2c_delay_5usec(uint16_t x)
{
    uint16_t j;
    uint8_t  i;
      
    for (j = 0; j < x; j++)
    {
        for (i = 0; i < 15; i++) ; // 15 results in 5.42 us on Logic-Analyzer
    } // for j
} // i2c_delay_5usec()
    
/*-----------------------------------------------------------------------------
  Purpose  : This function resets the I2C-bus after a lock-up. See also:
             http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html
  Variables: --
  Returns  : 0 if bus cleared
             1 if SCL held low, 
             2 if SDA held low by slave clock strecht for > 2 seconds
			 3 if SDA held low after 20 clocks
  ---------------------------------------------------------------------------*/
uint8_t i2c_reset_bus(void)
{
    int8_t   scl_cnt,sda_cnt; // must be a signed int!
    uint8_t  scl_low,sda_low;
    
    SDA_in;
    SCL_in; // SDA and SCL both input with external pull-ups
    i2c_delay_5usec(400);           // delay 2 msec.
    
    if (!SCL_read) // check if SCL is low
    {      	// if it is held low, the uC cannot become the I2C master
        return 1; // I2C bus-error. Could not clear SCL clock line held low
    } // if
    sda_low = !SDA_read;
    sda_cnt = 20; // > 2x9 clock
    while (sda_low && (sda_cnt-- > 0))
    {	// Note: I2C bus is open collector so do NOT drive SCL or SDA high.
        SCL_out; // SCL is Push-Pull output
        SCL_0;   // Set SCL low
        i2c_delay_5usec(1); // extra delay, so that even the slowest I2C devices are handled
        SCL_in; // SCL is input
        i2c_delay_5usec(2); // for > 5 us, so that even the slowest I2C devices are handled
        scl_low = !SCL_read; // check if SCL is low
        scl_cnt = 20;
        while (scl_low && (scl_cnt-- > 0))
        {
            i2c_delay_5usec(20000);        // delay 100 msec.
            scl_low = !SCL_read; // check if SCL is low
        } // while
        if (scl_low)
        {	// still low after 2 sec error
            return 2; // I2C error, could not clear, SCL held low by slave clock stretch for > 2 sec.
        } // if
        sda_low = !SDA_read;
    } // while
    if (sda_low)
    {	// still low
        return 3; // I2C bus error, could not clear, SDA held low
    } // if
    // else pull SDA line low for Start or Repeated Start
    SDA_1;   // Set SDA to 1
    SDA_out; // Set SDA to output
    SDA_0;   // Then make it low i.e. send an I2C Start or Repeated Start
    // When there's only one I2C master a Start or Repeat Start has the same function as a Stop
    // and clears the bus. A repeat Start is a Start occurring after a Start with no intervening Stop.
    i2c_delay_5usec(2); // delay 10 usec.
    SDA_1;              // Make SDA line high i.e. send I2C STOP control
    i2c_delay_5usec(2); // delay 10 usec.
    SCL_1;              // Set SCL to 1
    SCL_out;            // SCL is Push-Pull output
    return 0;           // all is good
} // i2c_reset_bus()

/*-----------------------------------------------------------------------------
  Purpose  : This function initializes the I2C bus 
  Variables: -
  Returns  : --
  ---------------------------------------------------------------------------*/
void i2c_init_bb(void)
{
    SDA_1; 
    SCL_1;
} // i2c_init()

/*-----------------------------------------------------------------------------
  Purpose  : This function generates an I2C start condition.
             This is defined as SDA 1 -> 0 while SCL = 1
  Variables: address: I2C address of device
  Returns  : ack bit from I2C device: ack (0) or nack (1)
  ---------------------------------------------------------------------------*/
uint8_t i2c_start_bb(uint8_t address)
{   // Pre-condition : SDA = 1
    SCL_1;     // SCL = 1
    SDA_0;     // SDA = 0
    i2c_delay_5usec(2); // delay 10 usec.
    SCL_0;     // SCL = 0
    // Post-condition: SCL = 0, SDA = 0
    return i2c_write_bb(address);
} // i2c_start_bb;

/*-----------------------------------------------------------------------------
  Purpose  : This function generates an I2C repeated-start condition
  Variables: address: I2C address of device
  Returns  : ack bit from I2C device: ack (0) or nack (1)
  ---------------------------------------------------------------------------*/
uint8_t i2c_rep_start_bb(uint8_t address)
{   
    SDA_1;
    i2c_delay_5usec(2); // delay 10 usec.
    return i2c_start_bb(address);
} // i2c_start_bb;

/*-----------------------------------------------------------------------------
  Purpose  : This function generates an I2C stop condition
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void i2c_stop_bb(void)
{   // Pre-condition : SDA = 0
    SCL_1;
    SDA_1;
    i2c_delay_5usec(2); // delay 5 usec.
} // i2c_stop_bb;

/*-----------------------------------------------------------------------------
  Purpose  : This function writes a byte to the I2C bus
  Variables: -
  Returns  : ack bit from I2C device: ack (0) or nack (1)
  ---------------------------------------------------------------------------*/
uint8_t i2c_write_bb(uint8_t data)
{
    uint8_t i   = 0x80;
    uint8_t ack = I2C_ACK;
    
    SCL_0; // clock low
    while (i > 0)
    {   // write bits to I2C bus
        if (data & i) SDA_1;
        else          SDA_0;
        SCL_1;
        SCL_0;
        i >>= 1;
    } // while
    SDA_in;             // set as input
    i2c_delay_5usec(2); // delay 5 usec.
    SCL_1;
    if (SDA_read) ack = I2C_NACK; // nack (1) 
    SCL_0;   // SCL = 0
    SDA_out; // set to output again
    SDA_0;   // SDA = 0
    return ack;
} // i2c_write_bb()

/*-----------------------------------------------------------------------------
  Purpose  : This function reads a byte from the I2C bus
  Variables: ack: send ack (0) or nack (1) to I2C device
  Returns  : result: byte read from I2C device
  ---------------------------------------------------------------------------*/
uint8_t i2c_read_bb(uint8_t ack)
{
    uint8_t result = 0x00;
    uint8_t i      = 0x80;
    
    SCL_0;  // clock low
    SDA_in; // set as input
    while (i > 0)
    {   // read bits from I2C bus
        result <<= 1; // make room for new bit
        SCL_1;        // SCL = 1
        if (SDA_read) result |=  0x01;
        SCL_0;        // SCL = 0
        i >>= 1;
    } // while
    SDA_out; // set to output again
    if (ack == I2C_ACK) 
         SDA_0; // output ACK
    else SDA_1; // output NACK
    SCL_1;      // SCL = 1
    SCL_0;      // SCL = 0
    SDA_0;      // SDA = 0
    return result;
} // i2c_read_bb()
