/*==================================================================
  File Name    : w3230_main.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the main() function and all the 
            hardware related functions for the STM8S105C6T6 uC.
            It is meant for the THD-Analyzer Control Board.
  ------------------------------------------------------------------
  This file is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this file.  If not, see <http://www.gnu.org/licenses/>.
  ==================================================================
*/ 
#include <intrinsics.h> 
#include <stdio.h>
#include "thd_analyzer_main.h"
#include "scheduler.h"
#include "adc.h"
#include "eep.h"
#include "i2c_bb.h"
#include "comms.h"
#include "uart.h"
#include "lcd_i2c.h"

// Version number for THD-Analyzer firmware
char version[] = "THD-Control-stm8s105c6 V0.10\n";

int16_t lvl_out;   // Sine-wave output level, ADC1
int16_t lvl_in;    // Input-level, ADC2
int16_t lvl_dist;  // Distortion level, ADC3
float   freq_meas; // Actual measured frequency

uint8_t  freq_sine;          // [FREQ_20_HZ, ..., FREQ_200_KHZ]
uint32_t pcb1_bits = 0L;     // 32 bits for PCB1 Shift-registers
uint32_t pcb2_bits = 0L;     // 32 bits for PCB2 Shift-registers
uint16_t pcb3_bits = 0x0000; // 16 bits for PCB3 Shift-registers

extern uint8_t  rs232_inbuf[];

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Range of the sine-wave generator.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_range(uint8_t range)
{
    pcb1_bits &= ~RANGE_MASK; // clear bits for RANGE
    pcb2_bits &= ~RANGE_MASK; // clear bits for RANGE
    pcb3_bits &= ~(uint16_t)RANGE_MASK; // clear bits for RANGE
    switch (range)
    {
        case RANGE_200_HZ:
            pcb1_bits |= RANGE_200_HZ_MASK;
            pcb2_bits |= RANGE_200_HZ_MASK;
            pcb3_bits |= (uint16_t)RANGE_200_HZ_MASK;
            break;
        case RANGE_2_KHZ:
            pcb1_bits |= RANGE_2_KHZ_MASK;
            pcb2_bits |= RANGE_2_KHZ_MASK;
            pcb3_bits |= (uint16_t)RANGE_2_KHZ_MASK;
            break;
        case RANGE_20_KHZ:
            pcb1_bits |= RANGE_20_KHZ_MASK;
            pcb2_bits |= RANGE_20_KHZ_MASK;
            pcb3_bits |= (uint16_t)RANGE_20_KHZ_MASK;
            break;
        default: // 200 kHz, not on PCB2
            pcb1_bits |= RANGE_200_KHZ_MASK;
            pcb3_bits |= (uint16_t)RANGE_200_KHZ_MASK;
            break;
    } // switch
} // set_range()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Frequency of the sine-wave generator.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_frequency(uint8_t freq)
{
    uint8_t f = freq;
    if (f < FREQ_250_HZ)
    {   // 20 Hz .. 200 Hz
        set_range(RANGE_200_HZ);
    } // if
    else if (f < FREQ_2500_HZ)
    {   // 250 Hz .. 2 kHz
        set_range(RANGE_2_KHZ);
        f -= FREQ_200_HZ;
    } // else if
    else if (f < FREQ_25_KHZ)
    {   // 2.5 kHz .. 20 kHz
        set_range(RANGE_20_KHZ);
        f -= FREQ_2000_HZ;
    } // else if
    else 
    {   // 25 kHz .. 200 kHz
        set_range(RANGE_200_KHZ);
        f -= FREQ_20_KHZ;
    } // else
    pcb1_bits &= ~FREQ_MASK;           // clear all frequency bits
    pcb2_bits &= ~FREQ_MASK;           // clear all frequency bits
    pcb3_bits &= ~(uint16_t)FREQ_MASK; // clear all frequency bits
    if (f > FREQ_20_HZ)
    {   // 20 Hz requires all relays to be off, all other freqs should be switched
        pcb1_bits |= (1L << (f-1)); // switch proper relay
        pcb2_bits |= (1L << (f-1)); // switch proper relay
        pcb3_bits |= (1  << (f-1)); // switch proper relay
    } // if
    setup_timer1(f); // Setup Timer1 frequency/period measurement
    freq_sine = f;   // save actual frequency
} // set_frequency()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Output Level of 
             the sine-wave generator on PCB1.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_output_level(uint8_t lvl)
{
    pcb1_bits &= ~LEVEL_MASK; // clear all level bits
    switch (lvl)
    {
        case LEVEL_0V15:
            pcb1_bits |= LEVEL_0V15_MASK;
            break;
        case LEVEL_0V50:
            pcb1_bits |= LEVEL_0V50_MASK;
            break;
        case LEVEL_1V50:
            pcb1_bits |= LEVEL_1V50_MASK;
            break;
        case LEVEL_5V:
            pcb1_bits |= LEVEL_5V_MASK;
            break;
    } // switch
} // set_output_level()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Input Level for PCB2.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_input_level(uint8_t lvl)
{
    if (lvl <= INPUT_100V)
    {
        pcb2_bits &= ~INPUT_MASK; // clear all level bits
        pcb2_bits |= (1L << (19 + lvl));
    } // if
} // set_input_level()

/*-----------------------------------------------------------------------------
  Purpose  : This function sets the Sensitivity Level in % for PCB2 and PCB3
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void set_sensitivity(uint8_t sens)
{
    pcb2_bits &= ~SENS_MASK_PCB2; // clear all sensitivity bits
    pcb3_bits &= ~SENS_MASK_PCB3; // clear all sensitivity bits
    switch (sens)
    {
        case SENS_0_003: // 0.003 %
            pcb2_bits |= SENS_0_003_MASK;
            break;
        case SENS_0_010: // 0.01 %
            pcb2_bits |= SENS_0_010_MASK;
            break;
        case SENS_0_030: // 0.03 %
            pcb2_bits |= SENS_0_030_MASK;
            break;
        case SENS_0_100: // 0.1 %
            pcb2_bits |= SENS_0_100_MASK;
            break;
        case SENS_0_300: // 0.3 %
            pcb2_bits |= SENS_0_300_MASK;
            break;
        case SENS_1_000: // 1 %
            pcb2_bits |= SENS_1_000_MASK;
            break;
        case SENS_3_000: // 3 %
            pcb2_bits |= SENS_3_000_MASK;
            break;
        default: // SENS = 10 %
            pcb2_bits |= SENS_10_000_MASK;
            break;
    } // switch
    if (sens < SENS_0_300) pcb3_bits |= SENS_LT_0_300;
    if (sens > SENS_0_010) pcb3_bits |= SENS_GT_0_010;
} // set_sensitivity()

/*-----------------------------------------------------------------------------
  Purpose  : This function sends the bit-settings to the HC595 shift-registers
             on the 3 PCBs of the THD Analyzer.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void send_to_hc595(void)
{
    uint32_t colmask  = 0x00000001;  // start with bit 0 to send to shift-register
    uint16_t colmask3 = 0x0001;      // bit-mask for PCB3 shift-register
    uint8_t i = 0;
    
    //-------------------------------------------------------------------
    // Since PCB3 contains fewer shift-register bits than PCB1 and PCB2,
    // we first start by shifting the extra bits to PCB1 and PCB2. These
    // bits are shifted out again by the PCB3 shift-register.
    //-------------------------------------------------------------------
    while (i < MAX_BITS12 - MAX_BITS3)
    {   // shift bits to HC595 shift-registers
        SDIN1b    = (pcb1_bits & colmask) ? 1 : 0;
        SDIN2b    = (pcb2_bits & colmask) ? 1 : 0;
        SDIN3b    = 0; // dummy
        SHCPb     = 1; // set clock for shift-register;
        colmask <<= 1; // select next bit
        SHCPb     = 0; // set clock to 0 again
        i++;
    } // while
    colmask3 = 0x0001;
    while (i < MAX_BITS12)
    {   // shift MAX_BITS3 bits to all HC595 shift-registers
        SDIN1b     = (pcb1_bits & colmask)  ? 1 : 0;
        SDIN2b     = (pcb2_bits & colmask)  ? 1 : 0;
        SDIN3b     = (pcb3_bits & colmask3) ? 1 : 0;
        SHCPb      = 1; // set clock for shift-register;
        colmask  <<= 1; // select next bit for PCB1 and PCB2
        colmask3 <<= 1; // select next bit for PCB3
        SHCPb      = 0; // set clock to 0 again
        i++;
    } // while
    // Now clock bits from shift-registers to output-registers
    STCPb     = 1; // set clock to 1
    colmask <<= 1; // small delay
    STCPb     = 0; // set clock to 0 again
} // hc595_send()

/*-----------------------------------------------------------------------------
  Purpose  : This task is called every 500 msec. and reads the following
             analog signals:
             AIN1: Output Level of sine-wave
             AIN2: Input Level
             AIN3: Distortion Level
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void adc_task(void)
{
    lvl_out  = read_adc(ADC1);
    lvl_in   = read_adc(ADC2);
    lvl_dist = read_adc(ADC3);
} // adc_task()

/*-----------------------------------------------------------------------------
  Purpose  : This task is called every 100 msec. and calculates the actual
             frequency of the sine-wave, based on the measured clock-ticks.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void freq_task(void)
{
    char s[20];
    
    calc_freq(); // calculate frequency
    lcd_i2c_setCursor(0,1);
    lcd_i2c_print("Freq.:");
    lcd_i2c_setCursor(7,1);
    sprintf(s,"%9.2f Hz",freq_meas);
    lcd_i2c_print(s);
    lcd_i2c_setCursor(17,1);
    lcd_i2c_print("Hz");
    // TODO: and to 7-segment display 1
} // std_task()

/*-----------------------------------------------------------------------------
  Purpose  : This task is called every second and contains the main control
             task for the device. It also calls temperature_control() / 
             pid_ctrl() and one_wire_task().
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void ctrl_task(void)
{
} // ctrl_task()

/*-----------------------------------------------------------------------------
  Purpose  : This is the main entry-point for the entire program.
             It initialises everything, starts the scheduler and 
             dispatches tasks.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
int main(void)
{
    char    s[30];  // Needed for xputs()
    uint8_t clk;
    
    __disable_interrupt();
    clk = initialise_system_clock(HSE); // Set system-clock to 16 MHz
    uart_init();                        // UART1 init. to 57600,8,N,1
    setup_timer2(clk,FREQ_1KHZ);        // Set Timer 2 for 1 kHz interrupt frequency
    setup_gpio_ports();                 // Init. needed output-ports 
    i2c_init_bb();                      // Init. I2C bus
    
    // Initialise all tasks for the scheduler
    scheduler_init();                    // clear task_list struct
    add_task(adc_task ,"ADC",  0,  500); // every 500 msec.
    add_task(freq_task,"FRQ", 50,  100); // every 100 msec.
    add_task(ctrl_task,"CTL",200, 1000); // every second
    __enable_interrupt();
    lcd_i2c_init(0x4E,20,4,LCD_5x8DOTS); // Needs working interrupts!
    lcd_i2c_clear();
    lcd_i2c_setCursor(0,0);
    lcd_i2c_backlight_on();
    lcd_i2c_print(version);
    xputs(version); // print version number
    
    while (1)
    {   // background-processes
        dispatch_tasks();     // Run task-scheduler()
        switch (rs232_command_handler()) // run command handler continuously
        {
            case ERR_CMD: xputs("Cmd Error\n"); break;
            case ERR_NUM: sprintf(s,"Nr Error (%s)\n",rs232_inbuf);
                          xputs(s);  
                          break;
            default     : break;
        } // switch
    } // while
} // main()
