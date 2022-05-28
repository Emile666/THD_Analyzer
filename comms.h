/*==================================================================
  File Name    : comms.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the header file for comms.c.
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
#ifndef _COMMS_H_
#define _COMMS_H_

#include <stdint.h>

#define NO_ERR  (0x00)
#define ERR_CMD	(0x01)
#define ERR_NUM	(0x02)

void    i2c_scan(void);
uint8_t rs232_command_handler(void);
uint8_t execute_single_command(char *s);

#endif