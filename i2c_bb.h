/*==================================================================
   File Name    : i2c_bb.h
   Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the generic I2C related functions 
            for the STM8 uC. It is needed for every I2C device
	    connected to the STM8 uC.
  ------------------------------------------------------------------
  This is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this software.  If not, see <http://www.gnu.org/licenses/>.
  ================================================================== */ 
#ifndef _I2C_BB_H
#define _I2C_BB_H

#include <stdbool.h>
#include <intrinsics.h> 
#include <stdint.h>
#include "delay.h"
#include "stm8_hw_init.h"

#define SCL_in    (PE_DDR &= ~I2C_SCL) /* Set SCL to input */
#define SDA_in    (PE_DDR &= ~I2C_SDA) /* Set SDA to input */
#define SCL_out   (PE_DDR |=  I2C_SCL) /* Set SCL to output */
#define SDA_out   (PE_DDR |=  I2C_SDA) /* Set SDA to  output */
#define SCL_read  (PE_IDR &   I2C_SCL) /* Read from SCL */
#define SDA_read  (PE_IDR &   I2C_SDA) /* Read from SDA */
#define SCL_0     {PE_ODR &= ~I2C_SCL; i2c_delay_5usec(1);} /* Set SCL to 0 */
#define SCL_1     {PE_ODR |=  I2C_SCL; i2c_delay_5usec(1);} /* Set SCL to 1 */
#define SDA_1     (PE_ODR |=  I2C_SDA) 			    /* Set SDA to 1 */
#define SDA_0     (PE_ODR &= ~I2C_SDA) 			    /* Set SDA to 0 */

//----------------------------
// I2C defines
//----------------------------
#define I2C_ACK     (0)
#define I2C_NACK    (1)
#define I2C_ERROR   (2)
#define I2C_WRITE   (0)
#define I2C_READ    (1)
#define I2C_RETRIES (3)

//----------------------------
// I2C-peripheral routines
//----------------------------
void    i2c_delay_5usec(uint16_t x);    // Standard I2C bus delay
uint8_t i2c_reset_bus(void);
void    i2c_init_bb(void);              // Initializes the I2C Interface. Needs to be called only once
uint8_t i2c_start_bb(uint8_t addr);     // Issues a start condition and sends address and transfer direction
uint8_t i2c_rep_start_bb(uint8_t addr); // Issues a repeated start condition and sends address and transfer direction
void    i2c_stop_bb(void);              // Terminates the data transfer and releases the I2C bus
uint8_t i2c_write_bb(uint8_t data);     // Send one byte to I2C device
uint8_t i2c_read_bb(uint8_t ack);       // Read one byte from I2C device and calls i2c_stop()

#endif
