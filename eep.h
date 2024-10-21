/*==================================================================
  File Name    : eep.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This is the header-file for eep.c
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
#ifndef _EEP_H
#define _EEP_H
#include <stdint.h>

// EEPROM base address within STM8 uC
#define EEP_BASE_ADDR ((uint8_t *)0x4000)

/*---------------------------------------------------------------*/
/* EEPROM base address for full-scale values, sizeof(float) == 4 */
/*---------------------------------------------------------------*/
#define FS_LVL_IN_100V_ADDR  (0) /* Full-scale float LVL_IN 100 V */
#define FS_LVL_IN_30V_ADDR   (4) /* Full-scale float LVL_IN  30 V */
#define FS_LVL_IN_10V_ADDR   (8) /* Full-scale float LVL_IN  10 V */
#define FS_LVL_IN_3V_ADDR   (12) /* Full-scale float LVL_IN   3 V */
#define FS_LVL_IN_1V_ADDR   (16) /* Full-scale float LVL_IN   1 V */

#define FS_LVL_IN_100V_VAL (32.645570) /* value of 1+R48/49 */
#define FS_LVL_IN_30V_VAL  ( 9.263636) /* value of 1+R46/47 */
#define FS_LVL_IN_10V_VAL  ( 2.613744) /* value of 1+R44/45 */
#define FS_LVL_IN_3V_VAL   ( 1.000000) /* pass-through */
#define FS_LVL_IN_1V_VAL   ( 0.327869) /* value of R51/(R511+R52) */

// Function prototypes
uint16_t eeprom_read_config(uint8_t eeprom_address);
void     eeprom_write_config(uint8_t eeprom_address,uint16_t data);
float    eeprom_read_float(uint8_t eeprom_address);
void     eeprom_write_float(uint8_t eeprom_address, float data);

#endif
