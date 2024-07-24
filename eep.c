/*==================================================================
  File Name    : eep.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the EEPROM related functions 
            for the STM8 uC.
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
#include "eep.h"
#include "stm8s105.h"

/*-----------------------------------------------------------------------------
  Purpose  : This function reads a (16-bit) value from the STM8 EEPROM.
  Variables: eeprom_address: the index number within the EEPROM. An index number
                             is the n-th 16-bit variable within the EEPROM.
  Returns  : the (16-bit value)
  ---------------------------------------------------------------------------*/
uint16_t eeprom_read_config(uint8_t eeprom_address)
{
    uint16_t data;
    char    *address = (char *)EEP_BASE_ADDR; //  EEPROM base address.
    
    address += (eeprom_address << 1); // convert to byte-address in EEPROM
    data      = *address++;           // read MSB first
    data    <<= 8;                    // SHL 8
    data     |= *address;             // read LSB
    return data;                      // Return result
} // eeprom_read_config()

/*-----------------------------------------------------------------------------
  Purpose  : This function writes a (16-bit) value to the STM8 EEPROM.
  Variables: eeprom_address: the index number within the EEPROM. An index number
                             is the n-th 16-bit variable within the EEPROM.
             data          : 16-bit value to write to the EEPROM
  Returns  : -
  ---------------------------------------------------------------------------*/
void eeprom_write_config(uint8_t eeprom_address,uint16_t data)
{
    char *address = (char *)EEP_BASE_ADDR; //  EEPROM base address.

    // Avoid unnecessary EEPROM writes
    if (data == eeprom_read_config(eeprom_address)) return;

    address += (eeprom_address << 1); // convert to byte-address in EEPROM
    FLASH_DUKR = 0xae; // unlock EEPROM
    FLASH_DUKR = 0x56;
    while (!FLASH_IAPSR_DUL) ; // wait until EEPROM is unlocked
    *address++ = (char)((data >> 8) & 0xff); // write MSB
    *address   = (char)(data & 0xff);        // write LSB
    FLASH_IAPSR_DUL = 0;                     // write-protect EEPROM again
} // eeprom_write_config()
