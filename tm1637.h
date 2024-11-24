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
#define TM1637_READ_KEYS  (0x42) /* Data Read keys command */
#define TM1637_I2C_COMM2  (0xC0) /* Address Command Setting */
#define TM1637_I2C_COMM3  (0x80) /* Display Control */

#define DISP_MIN          (0x40) /* Minus sign for negative numbers */
#define DISP_OFF          (0x00) /* Display off */
#define DP1               (0x80) /* Decimal-point for digit 1 (MSB) */
#define DP2               (0x40) /* Decimal-point for digit 2       */
#define DP3               (0x20) /* Decimal-point for digit 3       */
#define DP4               (0x10) /* Decimal-point for digit 4 (LSB) */
#define DP_ON             (0x80) /* Decimal-point on                */
#define DP_OFF            (0x00) /* Disable all decimal-points      */

#define TM1637_KEY_UP      (0x00) /* Key between S1 and K1 */
#define TM1637_KEY_DOWN    (0x01) /* Key between S2 and K1 */
#define TM1637_KEY_LEFT    (0x02) /* Key between S3 and K1 */
#define TM1637_KEY_RIGHT   (0x03) /* Key between S4 and K1 */
#define TM1637_KEY_OK      (0x04) /* Key between S5 and K1 */
#define TM1637_KEY_FP10    (0x08) /* Key between S1 and K2 */
#define TM1637_KEY_FM10    (0x09) /* Key between S2 and K2 */
#define TM1637_KEY_PPPKRMS (0x0A) /* Key between S3 and K2 */
#define TM1637_KEY_DBPERC  (0x0B) /* Key between S4 and K2 */

void    tm1637_dio_pin_input(uint8_t ssd_nr);
void    tm1637_dio_pin_output(uint8_t ssd_nr);
void    tm1637_start(uint8_t ssd_nr);
void    tm1637_stop(uint8_t ssd_nr);
void    tm1637_set_brightness(uint8_t ssd_nr, uint8_t brightness, bool on);
void    tm1637_set_segments(uint8_t ssd_nr, uint8_t *segments, uint8_t length, uint8_t pos);
void    tm1637_show_nr_dec(uint8_t ssd_nr, int16_t num, bool leading_zero, uint8_t length, uint8_t pos);
void    tm1637_show_nr_dec_ex(uint8_t ssd_nr, int16_t num, uint8_t dots, bool lzero, uint8_t length, uint8_t pos, uint8_t leds);
bool    tm1637_write_byte(uint8_t ssd_nr, uint8_t b);
uint8_t tm1637_read_byte(uint8_t ssd_nr);
uint8_t tm1637_read_keys(uint8_t ssd_nr);

#endif