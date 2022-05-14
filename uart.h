/*==================================================================
  File Name    : uart.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the header file for uart.c.
	    Original source at: 
            https://lujji.github.io/blog/bare-metal-programming-stm8/
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
  ==================================================================
*/ 
#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include <stdbool.h>

#define F_CPU       (16000000L)
#define BAUDRATE       (57600L)
#define UART_BUFLEN        (40)

#define TX_BUF_SIZE (20)
#define RX_BUF_SIZE (20)

void    uart_init(void);
void    uart_write(uint8_t data);
uint8_t uart_read(void);
void    xputs(char *s);
bool    uart_kbhit(void); /* returns true if character in receive buffer */

#endif