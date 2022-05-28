/*==================================================================
  File Name    : delay.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains timing related functions.
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
#include "delay.h"
#include <intrinsics.h> 
#include <iostm8s105c6.h>

uint32_t t2_millis = 0L; // Updated in TMR2 interrupt

/*------------------------------------------------------------------
  Purpose  : This function returns the number of milliseconds since
	     power-up. It is defined in delay.h
  Variables: -
  Returns  : The number of milliseconds since power-up
  ------------------------------------------------------------------*/
uint32_t millis(void)
{
	unsigned long m;
	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	__disable_interrupt();
	m = t2_millis;
	__enable_interrupt();
	return m;
} // millis()

/*------------------------------------------------------------------
  Purpose  : This function waits a number of milliseconds.  
             Do NOT use this in an interrupt.
	     TODO: fails every 49.7 days
  Variables: 
         ms: The number of milliseconds to wait.
  Returns  : -
  ------------------------------------------------------------------*/
void delay_msec(uint16_t ms)
{
    uint8_t  i;
    uint32_t tmr;
    uint32_t start = millis();
    
    do
    {   
        for (i = 0; i < 100; i++) ;
        tmr = millis();
	if (tmr < start)
        {
            start = 0xffffffff - start;
            start++;
        } // if
    } while ((tmr - start) < ms);
} // delay_msec()

/*------------------------------------------------------------------
  Purpose  : This function reads the value of TMR1.
  Variables: -
  Returns  : the value from TMR1
  ------------------------------------------------------------------*/
uint16_t tmr1_val(void)
{
    uint8_t  h,l;
    uint16_t tmr;
    
    __disable_interrupt();
    h = TIM1_CNTRH;
    l = TIM1_CNTRL;
    __enable_interrupt();
    tmr   = h;
    tmr <<= 8;
    tmr  |= l;	
    return tmr;
} // tmr1_val()

/*------------------------------------------------------------------
  Purpose  : This function reads the value of TMR2 which runs at 1 MHz.
  Variables: -
  Returns  : the value from TMR2
  ------------------------------------------------------------------*/
uint16_t tmr2_val(void)
{
    uint8_t  h,l;
    uint16_t tmr;
    
    __disable_interrupt();
    h = TIM2_CNTRH;
    l = TIM2_CNTRL;
    __enable_interrupt();
    tmr   = h;
    tmr <<= 8;
    tmr  |= l;	
    return tmr;
} // tmr2_val()

/*------------------------------------------------------------------
  Purpose  : This function waits a number of microseconds.  
             Do NOT use this in an interrupt.
  Variables: 
         ms: The number of microseconds to wait.
  Returns  : -
  ------------------------------------------------------------------*/
void delay_usec(uint16_t us)
{
    uint16_t tmr;
    uint16_t start = tmr2_val();
    do 
    {
        tmr = tmr2_val();
        if (tmr < start) start = 1001 - start;
    } while ((tmr - start) < us);
} // delay_usec()
