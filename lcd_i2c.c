/*==================================================================
  File Name    : lcd_i2c.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains all routines for the I2C LCD-display.
            It is converted into a C-library from the C++ 
	    Arduino-LiquidCrystal-I2C-library written by Frank de Brabander.
   ------------------------------------------------------------------
   When the display powers up, it is configured as follows:

   1. Display clear
   2. Function set:
      DL = 1; 8-bit interface data
      N = 0; 1-line display
      F = 0; 5x8 dot character font
   3. Display on/off control:
      D = 0; Display off
      C = 0; Cursor off
      B = 0; Blinking off
   4. Entry mode set:
      I/D = 1; Increment by 1
      S = 0; No shift
  
   Note, however, that resetting the uC doesn't reset the LCD, so we
   can't assume that it is in that state when calling these functions.
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
#include "lcd_i2c.h"
#include "delay.h"
#include "i2c_bb.h"

uint8_t _addr;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _cols;
uint8_t _rows;
uint8_t _charsize;
uint8_t _backlightval;

/*-----------------------------------------------------------------------------
  Purpose  : This function initialises the LCD_I2C display and sets it in the
             correct begin state. Must be called before anything else is done.
  Variables: lcd_addr: I2C slave address of the LCD display
             lcd_cols: Number of columns your LCD display has
			 lcd_rows: Number of rows your LCD display has
			 charsize: The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
	_addr         = lcd_addr;
	_cols         = lcd_cols;
	_rows         = lcd_rows;
	_charsize     = charsize;
	_backlightval = LCD_BACKLIGHT;

	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	if (_rows > 1) _displayfunction |= LCD_2LINE;
	// for some 1 line displays you can select a 10 pixel high font
	if ((_charsize != 0) && (_rows == 1)) _displayfunction |= LCD_5x10DOTS;
	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	delay_msec(50);

	// Now we pull both RS and R/W low to begin commands
	lcd_i2c_expanderWrite(_backlightval); // reset expander and turn backlight off (Bit 8 =1)
	delay_msec(1000);

	// Put the LCD into 4 bit mode, this is according to
	// the hitachi HD44780 datasheet, figure 24, pg 46
	// We start in 8-bit mode, try to set 4-bit mode
	lcd_i2c_write4bits(0x03 << 4);
	delay_msec(5);              // wait min 4.1ms
	lcd_i2c_write4bits(0x03 << 4); // second try
	delay_msec(5);              // wait min 4.1ms
	lcd_i2c_write4bits(0x03 << 4); // third go!
	delay_usec(150);
	lcd_i2c_write4bits(0x02 << 4); // finally, set to 4-bit interface

	// set # lines, font size, etc.
	lcd_i2c_send(LCD_FUNCTIONSET | _displayfunction,IR);
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_i2c_display_on();
	lcd_i2c_clear(); // clear it off
	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	lcd_i2c_send(LCD_ENTRYMODESET | _displaymode,IR);
	lcd_i2c_home();
} // lcd_i2c_init()

/***********************************************/
/********** high level commands, for the user! */
/***********************************************/

/*-----------------------------------------------------------------------------
  Purpose  : This function removes all the characters currently shown. Next 
             print/write operation will start from the 1st position on LCD display.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_clear(void)
{
	lcd_i2c_send(LCD_CLEARDISPLAY,IR); // clear display, set cursor position to zero
	delay_msec(2);                     // this command takes a long time!
} // lcd_i2c_clear()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the cursor on the 1st position on the LCD display.
             Next print/write operation will start from there.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_home(void)
{
	lcd_i2c_send(LCD_RETURNHOME,IR);  // set cursor position to zero
	delay_msec(2);                    // this command takes a long time!
} // lcd_i2c_clear()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the cursor to a specific row,column position.
  Variables: col: the column number
             row: the row number
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) 
	{
		row = _rows-1;    // we count rows starting w/0
	} // if
	lcd_i2c_send(LCD_SETDDRAMADDR | (col + row_offsets[row]),IR);
} // lcd_i2c_setCursor()

/*-----------------------------------------------------------------------------
  Purpose  : This function turns the display off (quickly). Backlight state will 
             remain unchanged. All characters on the display will return, when 
			 the display is enabled again.
			 Note: this function should only be used after lcd_i2c_display_on() 
			       has been used!
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_display_off(void) 
{
	_displaycontrol &= ~LCD_DISPLAYON;
	lcd_i2c_send(LCD_DISPLAYCONTROL | _displaycontrol,IR);
} // lcd_i2c_display_off()

/*-----------------------------------------------------------------------------
  Purpose  : This function turns the display on (quickly). Backlight state will 
             remain unchanged. All characters on the display will return, when 
			 the display is enabled again.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_display_on(void) 
{
	_displaycontrol |= LCD_DISPLAYON;
	lcd_i2c_send(LCD_DISPLAYCONTROL | _displaycontrol,IR);
} // lcd_i2c_display_on()

/*-----------------------------------------------------------------------------
  Purpose  : This function turns the underline cursor off
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_cursor_off(void) 
{
	_displaycontrol &= ~LCD_CURSORON;
	lcd_i2c_send(LCD_DISPLAYCONTROL | _displaycontrol,IR);
} // lcd_i2c_cursor_off()

/*-----------------------------------------------------------------------------
  Purpose  : This function shows a cursor indicator, a cursor can blink or not. 
             Use lcd_i2c_blink_on() & lcd_i2c_blink_off() for changing this.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_cursor_on(void) 
{
	_displaycontrol |= LCD_CURSORON;
	lcd_i2c_send(LCD_DISPLAYCONTROL | _displaycontrol,IR);
} // lcd_i2c_cursor_on()

/*-----------------------------------------------------------------------------
  Purpose  : This function disables blinking of the cursor indicator
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_blink_off(void) 
{
	_displaycontrol &= ~LCD_BLINKON;
	lcd_i2c_send(LCD_DISPLAYCONTROL | _displaycontrol,IR);
} // lcd_i2c_blink_off()

/*-----------------------------------------------------------------------------
  Purpose  : This function enables blinking of the cursor indicator
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_blink_on(void) 
{
	_displaycontrol |= LCD_BLINKON;
	lcd_i2c_send(LCD_DISPLAYCONTROL | _displaycontrol,IR);
} // lcd_i2c_blink_on()

// These commands scroll the display without changing the RAM
/*-----------------------------------------------------------------------------
  Purpose  : This function scrolls the display to the left without changing RAM
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_scrollDisplayLeft(void) 
{
	lcd_i2c_send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT,IR);
} // lcd_i2c_scrollDisplayLeft()

/*-----------------------------------------------------------------------------
  Purpose  : This function scrolls the display to the right without changing RAM
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_scrollDisplayRight(void) 
{
	lcd_i2c_send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT,IR);
} // lcd_i2c_scrollDisplayRight()

/*-----------------------------------------------------------------------------
  Purpose  : This function is for text that flows from Left to Right
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_leftToRight(void) 
{
	_displaymode |= LCD_ENTRYLEFT;
	lcd_i2c_send(LCD_ENTRYMODESET | _displaymode,IR);
} // lcd_i2c_leftToRight()

/*-----------------------------------------------------------------------------
  Purpose  : This function is for text that flows from Right to Left
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_rightToLeft(void) 
{
	_displaymode &= ~LCD_ENTRYLEFT;
	lcd_i2c_send(LCD_ENTRYMODESET | _displaymode,IR);
} // lcd_i2c_rightToLeft()

/*-----------------------------------------------------------------------------
  Purpose  : This function will 'right justify' text from the cursor
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_autoscroll_on(void) 
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	lcd_i2c_send(LCD_ENTRYMODESET | _displaymode,IR);
} // lcd_i2c_autoscroll_on()

/*-----------------------------------------------------------------------------
  Purpose  : This function will 'left justify' text from the cursor
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_autoscroll_off(void) 
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_i2c_send(LCD_ENTRYMODESET | _displaymode,IR);
} // lcd_i2c_autoscroll_off()

/*-----------------------------------------------------------------------------
  Purpose  : This function fills the first 8 CGRAM locations with custom chars
  Variables: location: [0..7], the location in CGRAM to write to
             charmap : pointer to array with bit-definition for character
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_createChar(uint8_t location, uint8_t *charmap) 
{
	uint8_t i;
	
	location &= 0x7; // we only have 8 locations 0-7
	lcd_i2c_send(LCD_SETCGRAMADDR | (location << 3),IR);
	for (i = 0; i < 8; i++) 
	{
		lcd_i2c_send(charmap[i],DR); // Select Data-Register
	} // for 
} // i2c_lcd_createChar()

/*-----------------------------------------------------------------------------
  Purpose  : This function turns the (optional) backlight off
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_backlight_off(void) 
{
	_backlightval = LCD_NOBACKLIGHT;
	lcd_i2c_expanderWrite(0);
} // lcd_i2c_backlight_off()

/*-----------------------------------------------------------------------------
  Purpose  : This function turns the (optional) backlight on
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_backlight_on(void) 
{
	_backlightval = LCD_BACKLIGHT;
	lcd_i2c_expanderWrite(0);
} // lcd_i2c_backlight_on()

/*-----------------------------------------------------------------------------
  Purpose  : This function returns the current status of the (optional) backlight
  Variables: -
  Returns  : false: backlight is off, true: backlight is on
  ---------------------------------------------------------------------------*/
bool lcd_i2c_getBacklight(void) 
{
  return (_backlightval == LCD_BACKLIGHT);
} // lcd_i2c_getBacklight()

/*******************************************************/
/************ low level data pushing commands **********/
/*******************************************************/

/*-----------------------------------------------------------------------------
  Purpose  : This function writes a command or data-byte to the I2C LCD display
  Variables: value: the byte to write
             mode : Instruction Register (0) or Data Register (1) byte
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_send(uint8_t value, uint8_t mode) 
{
	uint8_t highnib = value & 0xF0;
	uint8_t lownib  = (value << 4) & 0xF0;

	lcd_i2c_write4bits((highnib) | mode);
	lcd_i2c_write4bits((lownib) | mode);
} // lcd_i2c_send()

/*-----------------------------------------------------------------------------
  Purpose  : This function writes a nibble to the I2C LCD display
  Variables: value: the nibble to write
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_write4bits(uint8_t value) 
{
	lcd_i2c_expanderWrite(value);
	lcd_i2c_pulseEnable(value);
} // lcd_i2c_write4bits()

/*-----------------------------------------------------------------------------
  Purpose  : This function writes a nibble to the I2C bus
  Variables: _data: the nibble to write
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_expanderWrite(uint8_t _data)
{
	i2c_start_bb(_addr);
	i2c_write_bb(_data | _backlightval);
	i2c_stop_bb();
} // lcd_i2c_expanderWrite()

/*-----------------------------------------------------------------------------
  Purpose  : This function writes the enable pulse to the I2C LCD display
  Variables: _data: the data-byte to write
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_pulseEnable(uint8_t _data)
{
	lcd_i2c_expanderWrite(_data | En); // En high
	delay_usec(1);		           // enable pulse must be >450ns

	lcd_i2c_expanderWrite(_data & ~En); // En low
	delay_usec(50);			    // commands need > 37us to settle
} // lcd_i2c_pulseEnable()

/*-----------------------------------------------------------------------------
  Purpose  : This function writes a text-string to the I2C LCD display
  Variables: *s: pointer to the data-buffer to read from
  Returns  : -
  ---------------------------------------------------------------------------*/
void lcd_i2c_print(char *s)
{
    while (*s) lcd_i2c_send(*s++,DR);
} // lcd_i2c_print()
