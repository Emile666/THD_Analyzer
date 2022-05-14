/*==================================================================
  File Name    : uart.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the uart routines. Original source
            at: https://lujji.github.io/blog/bare-metal-programming-stm8/
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
  along with thisd file. If not, see <http://www.gnu.org/licenses/>.
  ==================================================================
*/ 
#include <iostm8s105c6.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <intrinsics.h>
#include "uart.h"
#include "ring_buffer.h"
#include "delay.h"

// buffers for use with the ring buffer (belong to the USART)
bool     ovf_buf_in; // true = input buffer overflow
uint16_t isr_cnt = 0;

struct ring_buffer ring_buffer_out;
struct ring_buffer ring_buffer_in;
uint8_t            out_buffer[TX_BUF_SIZE];
uint8_t            in_buffer[RX_BUF_SIZE];

uint8_t ch;       // debug
uint8_t uart2_sr; // debug

//-----------------------------------------------------------------------------
// UART Transmit complete Interrupt.
//
// This interrupt will be executed when the TXE (Transmit Data Register Empty)
// bit in UART2_SR is set. The TXE bit is set by hardware when the contents of 
// the TDR register has been transferred into the shift register. An interrupt 
// is generated if the TIEN bit =1 in the UART_CR2 register. It is cleared by a
// write to the UART_DR register.
//-----------------------------------------------------------------------------
#pragma vector=UART2_T_TXE_vector
__interrupt void UART_TX_IRQHandler(void)
{
    if (!ring_buffer_is_empty(&ring_buffer_out))
    {   // if there is data in the ring buffer, fetch it and send it
        UART2_DR = ring_buffer_get(&ring_buffer_out);
    } // if
    else
    {   // no more data to send, turn off interrupt
        UART2_CR2_TIEN = 0;
    } // else
} /* UART_TX_IRQHandler() */

//-----------------------------------------------------------------------------
// UART Receive Complete Interrupt.

// This interrupt will be executed when the RXNE (Read Data-Register Not Empty)
// bit in UART2_SR is set. This bit is set by hardware when the contents of the 
// RDR shift register has been transferred to the UART2_DR register. An interrupt 
// is generated if RIEN=1 in the UART_CR2 register. It is cleared by a read to 
// the UART2_DR register. It can also be cleared by writing 0.
//-----------------------------------------------------------------------------
#pragma vector=UART2_R_RXNE_vector
__interrupt void UART_RX_IRQHandler(void)
{
    volatile uint8_t ch;
    
    if (!ring_buffer_is_full(&ring_buffer_in))
    {
        ring_buffer_put(&ring_buffer_in, UART2_DR);
        ovf_buf_in = false;
    } // if
    else
    {
        uart2_sr   = UART2_SR; // Clear IDLE and Overrun errors
        ch         = UART2_DR; // clear RXNE flag
        ovf_buf_in = true;
    } // else
    isr_cnt++;
} /* UART_RX_IRQHandler() */

/*------------------------------------------------------------------
  Purpose  : This function initializes the UART to 115200,N,8,1
             Master clock is 16 MHz, baud-rate is 115200 Baud.
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
void uart_init(void)
{
    uint16_t div = (F_CPU + BAUDRATE / 2) / BAUDRATE;

    //  Clear the Idle Line Detected bit in the status register by a read
    //  to the UART1_SR register followed by a Read to the UART1_DR register.
    uint8_t tmp = UART2_SR;
    tmp = UART2_DR;

    //  Reset the UART registers to the reset values.
    UART2_CR1 = 0;
    UART2_CR2 = 0;
    UART2_CR4 = 0;
    UART2_CR3 = 0;
    UART2_GTR = 0;
    UART2_PSCR = 0;

    // initialize the in and out buffer for the UART
    ring_buffer_out = ring_buffer_init(out_buffer, TX_BUF_SIZE);
    ring_buffer_in  = ring_buffer_init(in_buffer , RX_BUF_SIZE);

    UART2_CR1_M    = 0;     //  8 Data bits.
    UART2_CR1_PCEN = 0;     //  Disable parity.
    UART2_CR3_STOP = 0;     //  1 stop bit.
    UART2_BRR2 = ((div >> 8) & 0xF0) + (div & 0x0F);
    UART2_BRR1 = div >> 4;

    //  Disable the transmitter and receiver.
    UART2_CR2_TEN = 0;      //  Disable transmit.
    UART2_CR2_REN = 0;      //  Disable receive.

    //  Set the clock polarity, clock phase and last bit clock pulse.
    UART2_CR3_CPOL = 0;
    UART2_CR3_CPHA = 0;
    UART2_CR3_LBCL = 0;

    //  Turn on the UART transmit, receive and the UART clock.
    UART2_CR2_TIEN = 1; // Enable Transmit interrupt
    UART2_CR2_RIEN = 1; // Enable Receive interrupt
    UART2_CR2_TEN  = 1; // Enable Transmitter
    UART2_CR2_REN  = 1; // Enable Receiver
    UART2_CR3_CKEN = 0; // set to 0 or receive will not work!!
} // uart_init()

/*------------------------------------------------------------------
  Purpose  : This function writes one data-byte to the uart.	
  Variables: data: the byte to send to the uart.
  Returns  : -
  ------------------------------------------------------------------*/
void uart_write(uint8_t data)
{
    while (ring_buffer_is_full(&ring_buffer_out)) delay_msec(1);
    __disable_interrupt(); // Disable interrupts to get exclusive access to ring_buffer_out
    if (ring_buffer_is_empty(&ring_buffer_out))
    {
        UART2_CR2_TIEN = 1; // First data in buffer, enable data ready interrupt
    } // if
    ring_buffer_put(&ring_buffer_out, data); // Put data in buffer
    __enable_interrupt();                    // Re-enable interrupts
} // uart_write()

/*------------------------------------------------------------------
  Purpose  : This function reads one data-byte from the uart.	
  Variables: -
  Returns  : the data-byte read from the uart
  ------------------------------------------------------------------*/
uint8_t uart_read(void)
{
    return ring_buffer_get(&ring_buffer_in);
} // uart_read()

/*------------------------------------------------------------------
  Purpose  : This function checks if a character is present in the
             receive buffer.
  Variables: -
  Returns  : 1 if a character is received, 0 otherwise
  ------------------------------------------------------------------*/
bool uart_kbhit(void) /* returns true if character in receive buffer */
{
    return !ring_buffer_is_empty(&ring_buffer_in);
} // uart_kbhit()

/*------------------------------------------------------------------
  Purpose  : This function writes a string to serial port 0, using
             the xputc() routine.
  Variables:
         s : The string to write to serial port 0
  Returns  : the number of characters written
  ------------------------------------------------------------------*/
void xputs(char *s)
{
    char *ch = s;

    while (*ch) 
    {
        if (*ch == '\n') uart_write('\r'); // add CR
        uart_write(*ch++); // Grab the next character
    } // while
} // xputs()

