/*==================================================================
  File Name    : lcd_i2c.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This file is the header-file for lcd_i2c.c
  ------------------------------------------------------------------
  This is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This file is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this file. If not, see <http://www.gnu.org/licenses/>.
  ==================================================================*/
#ifndef _LCD_I2C_H_
#define _LCD_I2C_H_

#include <stdbool.h>
#include <stdint.h>

// commands
#define LCD_CLEARDISPLAY        (0x01)
#define LCD_RETURNHOME          (0x02)
#define LCD_ENTRYMODESET        (0x04)
#define LCD_DISPLAYCONTROL      (0x08)
#define LCD_CURSORSHIFT         (0x10)
#define LCD_FUNCTIONSET         (0x20)
#define LCD_SETCGRAMADDR        (0x40)
#define LCD_SETDDRAMADDR        (0x80)

// flags for display entry mode
#define LCD_ENTRYRIGHT          (0x00)
#define LCD_ENTRYLEFT           (0x02)
#define LCD_ENTRYSHIFTINCREMENT (0x01)
#define LCD_ENTRYSHIFTDECREMENT (0x00)

// flags for display on/off control
#define LCD_DISPLAYON           (0x04)
#define LCD_DISPLAYOFF          (0x00)
#define LCD_CURSORON            (0x02)
#define LCD_CURSOROFF           (0x00)
#define LCD_BLINKON             (0x01)
#define LCD_BLINKOFF            (0x00)

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         (0x08)
#define LCD_CURSORMOVE          (0x00)
#define LCD_MOVERIGHT           (0x04)
#define LCD_MOVELEFT            (0x00)

// flags for function set
#define LCD_8BITMODE            (0x10)
#define LCD_4BITMODE            (0x00)
#define LCD_2LINE               (0x08)
#define LCD_1LINE               (0x00)
#define LCD_5x10DOTS            (0x04)
#define LCD_5x8DOTS             (0x00)

// flags for backlight control
#define LCD_BACKLIGHT           (0x08)
#define LCD_NOBACKLIGHT         (0x00)

#define En (0x04) /* Enable bit */
#define Rw (0x02) /* Read/Write bit */
#define DR (0x01) /* Data Register */
#define IR (0x00) /* Instruction Register */

void lcd_i2c_init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize);
void lcd_i2c_clear(void);
void lcd_i2c_home(void);
void lcd_i2c_setCursor(uint8_t col, uint8_t row);
void lcd_i2c_display_off(void);
void lcd_i2c_display_on(void);
void lcd_i2c_cursor_off(void);
void lcd_i2c_cursor_on(void);
void lcd_i2c_blink_off(void);
void lcd_i2c_blink_on(void);

void lcd_i2c_scrollDisplayLeft(void);
void lcd_i2c_scrollDisplayRight(void);
void lcd_i2c_leftToRight(void);
void lcd_i2c_rightToLeft(void);
void lcd_i2c_autoscroll_on(void); 
void lcd_i2c_autoscroll_off(void); 
void lcd_i2c_createChar(uint8_t location, uint8_t *charmap);
void lcd_i2c_backlight_off(void); 
void lcd_i2c_backlight_on(void); 
bool lcd_i2c_getBacklight(void);
void lcd_i2c_print(char *s);

/*******************************************************/
/************ low level data pushing commands **********/
/*******************************************************/
void lcd_i2c_send(uint8_t value, uint8_t mode);
void lcd_i2c_write4bits(uint8_t value); 
void lcd_i2c_expanderWrite(uint8_t _data);
void lcd_i2c_pulseEnable(uint8_t _data);

#endif
