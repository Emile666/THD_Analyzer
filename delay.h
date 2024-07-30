/*==================================================================
  File Name    : delay.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the header file for delay.c.
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
#ifndef _DELAY_H
#define _DELAY_H

#include <stdint.h>

uint32_t millis(void);
void     delay_msec(uint16_t ms);
void     delay_usec(uint16_t us);
uint16_t tmr1_val(void);
void     tmr1_reset(void);
uint16_t tmr2_val(void);

#endif