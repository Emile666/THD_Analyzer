/*==================================================================
  File Name    : tm1637.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the header file for tm1637.c.
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
#ifndef _TM1637_H_
#define _TM1637_H_

#include <stdbool.h>
#include "stm8_hw_init.h"

#define TM1637_DELAY_USEC    (2)
#define TM1637_I2C_COMM1  (0x40) /* Data Write Mode Setting */
#define TM1637_I2C_COMM2  (0xC0) /* Address Command Setting */
#define TM1637_I2C_COMM3  (0x80) /* Display Control */

void tm1637_dio_pin_input(uint8_t ssd_nr);
void tm1637_dio_pin_output(uint8_t ssd_nr);
void tm1637_start(uint8_t ssd_nr);
void tm1637_stop(uint8_t ssd_nr);
void tm1637_set_brightness(uint8_t ssd_nr, uint8_t brightness, bool on);
void tm1637_set_segments(uint8_t ssd_nr, uint8_t *segments, uint8_t length, uint8_t pos);
void tm1637_show_nr_dec(uint8_t ssd_nr, int num, bool leading_zero, uint8_t length, uint8_t pos);
void tm1637_show_nr_dec_ex(uint8_t ssd_nr, int num, uint8_t dots, bool lzero, uint8_t length, uint8_t pos);
bool tm1637_write_byte(uint8_t ssd_nr, uint8_t b);

#endif