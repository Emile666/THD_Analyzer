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
  Variables: --
  Returns  : -
  ---------------------------------------------------------------------------*/
void i2c_delay_5usec(uint16_t x)
{
    uint16_t j;
    uint8_t  i;
      
    for (j = 0; j < x; j++)
    {
        for (i = 0; i < 80; i++) ; // 80 * 62.5 nsec (16 MHz) = 5 usec.
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
  Purpose  : This function initializes the I2C bit-banging routines
  Variables: -
  Returns  : --
  ---------------------------------------------------------------------------*/
void i2c_init_bb(void)
{
    SDA_1;   // Set SDA to 1
    SCL_1;   // Set SCL to 1
    SCL_out; // SCL is Push-Pull output
    SDA_out; // SDA line is Push-Pull output
} // i2c_init_bb()

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
    i2c_delay_5usec(1); // delay 5 usec.
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
    i2c_delay_5usec(1); // delay 5 usec.
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
    i2c_delay_5usec(1); // delay 5 usec.
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
    i2c_delay_5usec(1); // delay 5 usec.
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

//--------------------------------------------------------------------------
// Perform a device reset on the DS2482
//
// Device Reset
//   S AD,0 [A] DRST [A] Sr AD,1 [A] [SS] A\ P
//  [] indicates from slave
//  SS status byte to read to verify state
//
// Input: addr: the I2C address of the DS2482 to reset
// Returns: TRUE if device was reset
//          FALSE device not detected or failure to perform reset
//--------------------------------------------------------------------------
int8_t ds2482_reset(uint8_t addr)
{
    uint8_t err, ret = 0;
    
    // generate I2C start + output address to I2C bus
    err = (i2c_start_bb(addr | I2C_WRITE) == I2C_NACK);
    if (!err)
    {
        i2c_write_bb(CMD_DRST); // write register address
        i2c_rep_start_bb(addr | I2C_READ);
        ret = i2c_read_bb(I2C_NACK); // Read byte & generate I2C stop condition
        i2c_stop_bb();
    } // if
    // check for failure due to incorrect read back of status
    if (!err && ((ret & 0xF7) == 0x10))
         return true;
    else return false;	
} // ds2482_reset()

//--------------------------------------------------------------------------
// Write the configuration register in the DS2482. The configuration 
// options are provided in the lower nibble of the provided config byte. 
// The uppper nibble in bitwise inverted when written to the DS2482.
//  
// Write configuration (Case A)
//   S AD,0 [A] WCFG [A] CF [A] Sr AD,1 [A] [CF] A\ P
//  [] indicates from slave
//  CF configuration byte to write
//
// Input: addr: the I2C address of the DS2482 to reset
// Returns:  TRUE: config written and response correct
//           FALSE: response incorrect
//--------------------------------------------------------------------------
int8_t ds2482_write_config(uint8_t addr)
{
    uint8_t err, read_config = 0x00;
    
    // generate I2C start + output address to I2C bus
    err = (i2c_start_bb(addr | I2C_WRITE) == I2C_NACK);
    if (!err)
    {
        i2c_write_bb(CMD_WCFG); // write register address
        i2c_write_bb(DS2482_CONFIG); // write register address
        i2c_rep_start_bb(addr | I2C_READ);
        read_config = i2c_read_bb(I2C_NACK); // Read byte, generate I2C stop condition
        i2c_stop_bb();
    } // if
    // check for failure due to incorrect read back
    if (err || (read_config != (DS2482_CONFIG & 0x0f)))
    {
        ds2482_reset(addr); // handle error
        return false;
    } // if
    return true;
} // ds2482_write_config()

//--------------------------------------------------------------------------
// DS2428 Detect routine that performs a device reset followed by writing 
// the default configuration settings (active pullup enabled)
//
// Input: addr: the I2C address of the DS2482 to reset
// Returns: TRUE if device was detected and written
//          FALSE device not detected or failure to write configuration byte
//--------------------------------------------------------------------------
int8_t ds2482_detect(uint8_t addr)
{
   if (!ds2482_reset(addr)) // reset the DS2482
        return false;

   if (!ds2482_write_config(addr)) // write default configuration settings
        return false;
   else return true;
} // ds2482_detect()

//--------------------------------------------------------------------------
// Use the DS2482 help command '1-Wire triplet' to perform one bit of a 1-Wire
// search. This command does two read bits and one write bit. The write bit
// is either the default direction (all device have same bit) or in case of 
// a discrepancy, the 'search_direction' parameter is used. 
//
// Returns: The DS2482 status byte result from the triplet command
//--------------------------------------------------------------------------
uint8_t ds2482_search_triplet(uint8_t search_direction, uint8_t addr)
{
    uint8_t err, status;
    int poll_count = 0;
    
    // 1-Wire Triplet (Case B)
    //   S AD,0 [A] 1WT [A] SS [A] Sr AD,1 [A] [Status] A [Status] A\ P
    //                                         \--------/        
    //                           Repeat until 1WB bit has changed to 0
    //  [] indicates from slave
    //  SS indicates byte containing search direction bit value in msbit
    // generate I2C start + output address to I2C bus
    err = (i2c_start_bb(addr | I2C_WRITE) == I2C_NACK);
    if (!err)
    {
        i2c_write_bb(CMD_1WT); // write register address
        i2c_write_bb(search_direction ? 0x80 : 0x00);
        i2c_rep_start_bb(addr | I2C_READ);
        // loop checking 1WB bit for completion of 1-Wire operation 
        // abort if poll limit reached
        status = i2c_read_bb(I2C_ACK); // Read byte
        do
        {
            if (status & STATUS_1WB) status = i2c_read_bb(I2C_ACK);
        } while ((status & STATUS_1WB) && (poll_count++ < DS2482_OW_POLL_LIMIT));	
        i2c_read_bb(I2C_NACK); // Read byte, generate I2C stop condition	   
        i2c_stop_bb();
        // check for failure due to poll limit reached
        if (poll_count >= DS2482_OW_POLL_LIMIT)
        {
            ds2482_reset(addr); // handle error
            return false;
        } // if
        return status;
    } // if
    else return false;
} // ds2482_search_triplet()