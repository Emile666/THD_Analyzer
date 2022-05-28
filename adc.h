/*==================================================================
  File Name    : adc.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This is the header-file for adc.c
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
  along with this file.  If not, see <http://www.gnu.org/licenses/>.
  ==================================================================*/ 
#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>
#include <stdbool.h>

#define ADC_AVG      (4)

// Function prototypes
uint16_t read_adc(uint8_t ch);

#endif
