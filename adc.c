/*==================================================================
  File Name    : adc.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the ADC related functions 
            for the STM8 uC.
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
#include "adc.h"
#include <iostm8s105c6.h>

/*-----------------------------------------------------------------------------
  Purpose  : This routine reads a value from an ADC channel
 Variables : ch: channel number
  Returns  : the value read from the ADC
  ---------------------------------------------------------------------------*/
uint16_t read_adc(uint8_t ch)
{
    uint16_t result1 = 0; // conversion result
    uint16_t result2 = 0; // sum of results
    uint8_t  i,resultL;
    
    // From the STM8 Reference Manual:
    // When the ADC is powered on, the digital input and output stages of the selected channel
    // are disabled independently on the GPIO pin configuration. It is therefore recommended to
    // select the analog input channel before powering on the ADC
    // Time needed: tSTAB = 7 us, tCONV = 3.5 us (fADC = 4 MHz). Total = 10.5 us)
    ADC_CSR_CH    = ch;         // Select ADC channel
    ADC_CR1_ADON  = 1;          // Turn ADC on, note a 2nd set is required to start the conversion.
    ADC_CR3_DBUF  = 0;
    ADC_CR2_ALIGN = 1;          // Data is right aligned.
    while (ADC_CR1_ADON == 0) ; // wait until ADC is turned on
    
    for (i = 0; i < ADC_AVG; i++)
    {
        ADC_CR1_ADON  = 1;          // This 2nd write starts the conversion.
        while (ADC_CSR_EOC == 0) ;  // wait until conversion is complete
        resultL      = ADC_DRL;     // With right-alignment, LSB must be read first
        result1      = ADC_DRH;	    // read MSB of conversion result
        result1    <<= 8;
        result1     |= resultL;     // Add LSB and MSB together
        result2     += result1;     // Add results together
        ADC_CSR_EOC  = 0;	    // Reset conversion complete flag
    } // for
    ADC_CR1_ADON = 0;               // Disable the ADC
    result2 /=  ADC_AVG;            // Calculate average of samples
    return result2;
} // read_adc()


