/*==================================================================
  File Name: stm8_hw_init.c
  Author   : Emile
  ------------------------------------------------------------------
  Purpose  : This files initializes all the hardware peripherals 
             of the STM8S207R8 uC.
  ------------------------------------------------------------------
  This file is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this software. If not, see <http://www.gnu.org/licenses/>.
  ================================================================== */ 
#include "stm8_hw_init.h"
#include "scheduler.h"
#include "delay.h"
#include "THD_Analyzer_main.h"

extern uint32_t t2_millis;   // needed for delay_msec()
extern uint8_t  freq_sine;   // [FREQ_20_HZ, ..., FREQ_200_KHZ]
extern float    freq_meas;   // The measured frequency

int16_t  tmr1_ticks;         // sine wave period time in usec.
uint16_t tmr1;               // Timer1 value
uint16_t prev_tmr1;          // previous value of tmr1
int16_t  tim1_read_time = 0; // Time interval in msec. for reading timer 1
int16_t  tim1_read_cntr = 0; // Counter for reading timer 1

/*------------------------------------------------------------------
  Purpose  : This function calculates the actual frequency of the
             rectified signal and stores the result in freq_meas.
  Variables: f: the frequency set for the sine-wave generator
             tmr1_ticks: the measured clock_ticks difference of timer 1.
  Returns  : -
  ------------------------------------------------------------------*/
void calc_freq(void)
{
    if (freq_sine > FREQ_2000_HZ)
    {   // Frequency measurement, high-frequency, freq > 2 kHz
       switch (freq_sine)
       {
           case FREQ_2500_HZ:
           case FREQ_3000_HZ:
           case FREQ_4000_HZ: // read every 400 msec.
                freq_meas = 2.5 * tmr1_ticks;
                break;
           case FREQ_5000_HZ:
           case FREQ_6500_HZ:
           case FREQ_8000_HZ: // read every 200 msec.
                freq_meas = 5.0 * tmr1_ticks;
                break;
           default:           // read every 100 msec.
                freq_meas = 10.0 * tmr1_ticks;
                break;
       } // switch
    } // if
    else if (freq_sine <= FREQ_200_HZ)
    {   // Period measurement, low-frequency, freq <= 200 Hz, Timer 1 clock 400 kHz
        if (tmr1_ticks > 0)
             freq_meas = 400000.0 / tmr1_ticks;
        else freq_meas = 0.0;
    } // else if
    else
    {   // Period measurement, freq > 200 Hz && freq <= 2 kHz
        if (tmr1_ticks > 0)
             freq_meas = 4000000.0 / tmr1_ticks;
        else freq_meas = 0.0;
    } // else
} // calc_freq()

/*------------------------------------------------------------------
  Purpose  : This function reads the Timer 1 and calculates the
             difference between the previous value. It is an inline
             function, since it is called from two interrupt routines.
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
static inline void read_timer1_ticks(void)
{
    prev_tmr1  = tmr1;
    tmr1       = tmr1_val();
    if (tmr1 < prev_tmr1)
         tmr1_ticks = (uint16_t)(65535L + tmr1 - prev_tmr1);
    else tmr1_ticks = tmr1 - prev_tmr1;
} // read_timer1_ticks()

/*------------------------------------------------------------------
  Purpose  : This is the PORTC external interrupt routine. It is
             executed on every rising edge of PC1.
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
#pragma vector = EXTI2_vector
__interrupt void PORTC_EXT_IRQ(void)
{
    read_timer1_ticks(); // Read Timer 1 difference
} // PORTC_EXT_IRQ()

/*------------------------------------------------------------------
  Purpose  : This is the Timer-interrupt routine for the Timer 2 
             Overflow handler which runs every msec. (f=1 kHz). 
             It is used by the task-scheduler.
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
#pragma vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_UPD_OVF_IRQHandler(void)
{
    IRQ_LEDb = 1;      // Start Time-measurement
    t2_millis++;       // update millisecond counter
    scheduler_isr();   // call the ISR routine for the task-scheduler
    
    if (tim1_read_time > 0) // set by setup_timer1() function
    {   // frequency measurement of high-frequency signal
        if (++tim1_read_cntr >= tim1_read_time)
        {   // Time-out
            read_timer1_ticks(); // Read Timer 1 difference
            tim1_read_cntr = 0;  // reset counter
        } // if
    } // if
    IRQ_LEDb     = 0; // Stop Time-measurement
    TIM2_SR1_UIF = 0; // Reset the interrupt otherwise it will fire again straight away.
} // TIM2_UPD_OVF_IRQHandler()

/*-----------------------------------------------------------------------------
  Purpose  : This routine initialises Timer 1 for frequency and/or period measurement.
             frequency: this is used for frequency measurement of high-speed signals,
                        from 2 kHz up to 200 kHz. Timer 1 is clocked by an external
                        signal (TIM1_CH1->TI1) and the Timer 1 value is read and 
                        reset every N msec. in the Timer 2 interrupt 
             period:    this is used for frequency measurement of slow signals,
                        up to 2 kHz. The external interrupt from PC1/TIM1_CH1 is used
                        to read (and reset) the value of Timer 1.
  Variables: f: [FREQ_20_HZ, ..., FREQ_200_KHZ]: the frequency of the sine-wave
  Returns  : -
  ---------------------------------------------------------------------------*/
void setup_timer1(uint8_t f)
{
    TIM1_CR1_CEN = 0; // Disable the counter
    if (f > FREQ_2000_HZ)
    {	// Frequency measurement, high-frequency
        PC_CR2 &= ~FREQ;  // Disable interrupt for FREQ input pin
        // Par. 17.4.3 External clock source mode 1
        TIM1_CCMR1_CC1S = 1; // Configure ch.1 to detect rising edges on TI1 input
        TIM1_CCMR1 &= 0x0F;  // No input filter duration
        TIM1_CCER1_CC1P = 0; // Rising edge polarity
        TIM1_SMCR_SMS   = 7; // Select external clock mode 1
        TIM1_SMCR_TS    = 5; // Select TI1 as the input source
        TIM1_PSCRH      = 0; // Divide by 2, since signal is rectified (f doubles)
        TIM1_PSCRL      = 1; 
        switch (f)
        {
            case FREQ_2500_HZ:
            case FREQ_3000_HZ:
            case FREQ_4000_HZ: 
                 tim1_read_time = 400; // gives 1000, 1200 and 1600 clock-ticks
                 break;
            case FREQ_5000_HZ:
            case FREQ_6500_HZ:
            case FREQ_8000_HZ: 
                 tim1_read_time = 200; // gives 1000, 1300 and 1600 clock-ticks
                 break;
            default:
                 tim1_read_time = 100; // 10 kHz -> 1000 clock-ticks; 200 kHz -> 20.000 clock-ticks
                 break;
        } // switch
    } // if
    else // f <= FREQ_2000_HZ
    {   // TMEAS, Period measurement, low-frequency
        TIM1_SMCR_SMS = 0; // Par. 17.4.2, internal clock source (fmaster)
        TIM1_ETR_ECE  = 0;
        if (f <= FREQ_200_HZ)
        {   // Timer 1 clock 400 kHz: 20 Hz -> 20.000 ticks, 200 Hz -> 2.000 ticks
            TIM1_PSCRH = 0x0000; // 16 MHz / (39+1) = 400 kHz 
            TIM1_PSCRL = 0x0027; // 39 = 0x27
        } //
        else // f > 200 Hz && f <= 2 kHz
        {   // Timer 1 clock 4 MHz: 200 Hz -> 20.000 ticks, 2 kHz -> 2.000 ticks
            TIM1_PSCRH = 0x0000; // 16 MHz / (3+1) = 4 MHz 
            TIM1_PSCRL = 0x0003; // 3 = 0x03	
        } // else
        tim1_read_time = 0; // disable reading in interrupt
        PC_CR2 |=  FREQ;    // Enable interrupt for FREQ input pin
    } // else
    TIM1_CNTRH   = 0; // Reset counter MSB
    TIM1_CNTRL   = 0; // Reset counter LSB
    TIM1_CR1_CEN = 1; // Enable the counter
} // setup_timer1()

/*-----------------------------------------------------------------------------
  Purpose  : This routine initialises the system clock to run at 24 MHz.
             It uses the external HSE oscillator. 
             NOTE: For 24 MHz, set ST-LINK->Option Bytes...->Flash_Wait_states to 1
  Variables: clk: which oscillator to use: HSI (0xE1), HSE (0xB4) or LSI (0xD2)
  Returns  : which oscillator is active: HSI (0xE1), HSE (0xB4) or LSI (0xD2)
  ---------------------------------------------------------------------------*/
uint8_t initialise_system_clock(uint8_t clk)
{
    CLK_ECKR       = 0;           //  Reset the External Clock Register.
    CLK_ECKR_HSEEN = 1;           //  Enable the HSE.
    while (CLK_ECKR_HSERDY == 0); //  Wait for the HSE to be ready for use.
    CLK_CKDIVR     = 0;           //  Ensure the clocks are running at full speed.
 
    // The datasheet lists that the max. ADC clock is equal to 6 MHz (4 MHz when on 3.3V).
    // Because fMASTER is now at 24 MHz, we need to set the ADC-prescaler to 6.
    ADC_CR1_SPSEL  = 0x03;        //  Set prescaler to 6, fADC = 4 MHz
    CLK_SWIMCCR    = 0;           //  Set SWIM to run at clock / 2.
    while (CLK_SWCR_SWBSY != 0);  //  Pause while the clock switch is busy.

    //  Enable switching for 24 MHz
    if (clk == HSE) CLK_SWCR_SWEN  = 1;
    CLK_SWR        = 0xB4;        //  Use HSE as the clock source.
    while (CLK_SWCR_SWIF == 0);   //  Target clock source not ready  
    CLK_SWCR       = 0;           //  Reset the clock switch control register.
    CLK_SWCR_SWEN  = 1;           //  Enable switching.
    while (CLK_SWCR_SWBSY != 0);  //  Pause while the clock switch is busy.
    return CLK_CMSR;              //  Return which oscillator is active
} // initialise_system_clock()

/*-----------------------------------------------------------------------------
  Purpose  : This routine initialises Timer 2 to generate a 1, 2, 4 or 8 kHz interrupt.
  Variables: clk : [HSE,HSI]
             freq: [FREQ_1KHZ,FREQ_2KHZ,FREQ_4KHz,FREQ_8KHz]
  Returns  : -
  ---------------------------------------------------------------------------*/
void setup_timer2(uint8_t clk, uint8_t freq)
{
    //----------------------------------------------------------
    // Timer 2 values for an interrupt frequency of 1 kHz
    //   16 MHz: TIM2_ARRH=0x03, TIM2_ARRL=0xE8 (0x03E8 = 1000)
    //   24 MHz: TIM2_ARRH=0x05, TIM2_ARRL=0xDC (0x05DC = 1500)
    //----------------------------------------------------------
    // Timer 2 values for an interrupt frequency of 2 kHz
    //   16 MHz: TIM2_ARRH=0x01, TIM2_ARRL=0xF4 (0x01F4 =  500)
    //   24 MHz: TIM2_ARRH=0x02, TIM2_ARRL=0xEE (0x02EE =  750)
    //----------------------------------------------------------
    // Timer 2 values for an interrupt frequency of 4 kHz
    //   16 MHz: TIM2_ARRH=0x00, TIM2_ARRL=0xFA (0x00FA =  250)
    //   24 MHz: TIM2_ARRH=0x01, TIM2_ARRL=0x77 (0x0177 =  375)
    //----------------------------------------------------------
    // Timer 2 values for an interrupt frequency of 60*128 = 7680 Hz
    //   16 MHz: TIM2_ARRH=0x00, TIM2_ARRL=0x7D (0x007D =  125)
    //   24 MHz: TIM2_ARRH=0x00, TIM2_ARRL=0xBC (0x00BC =  188) 
    //----------------------------------------------------------
    TIM2_PSCR    = 0x04;  //  Prescaler = 16
    if (clk == HSE)
    {   // external 24 MHz HSE oscillator
        if (freq == FREQ_2KHZ)
        {
            TIM2_ARRH    = 0x02;  //  High byte for 24 MHz -> 2 kHZ
            TIM2_ARRL    = 0xEE;  //  Low  byte for 24 MHz -> 2 kHz
        } // if
        else if (freq == FREQ_4KHZ)
        {
            TIM2_ARRH    = 0x01;  //  High byte for 24 MHz -> 4 kHZ
            TIM2_ARRL    = 0x77;  //  Low  byte for 24 MHz -> 4 kHz
        } // else if
        else if (freq == FREQ_8KHZ)
        {
            //TIM2_ARRH    = 0x00;  //  High byte for 24 MHz -> 8 kHZ
            //TIM2_ARRL    = 0xBC;  //  Low  byte for 24 MHz -> 8 kHz
            TIM2_ARRH    = 0x00;  //  High byte for 24 MHz -> 8 kHZ
            TIM2_ARRL    = 0xC3;  //  Low  byte for 24 MHz -> 8 kHz
        } // else if
        else
        {   // FREQ_1KHZ
            TIM2_ARRH    = 0x05;  //  High byte for 24 MHz -> 1 kHZ
            TIM2_ARRL    = 0xDC;  //  Low  byte for 24 MHz -> 1 kHz
        } // else
    } // if
    else
    {   // internal 16 MHz HSI oscillator
        if (freq == FREQ_2KHZ)
        {
            TIM2_ARRH    = 0x01;  //  High byte for 16 MHz -> 2 kHZ
            TIM2_ARRL    = 0xF4;  //  Low  byte for 16 MHz -> 2 kHz
        } // if
        else if (freq == FREQ_4KHZ)
        {
            TIM2_ARRH    = 0x00;  //  High byte for 16 MHz -> 4 kHZ
            TIM2_ARRL    = 0xFA;  //  Low  byte for 16 MHz -> 4 kHz
        } // else if
        else if (freq == FREQ_8KHZ)
        {
            TIM2_ARRH    = 0x00;  //  High byte for 16 MHz -> 8 kHZ
            TIM2_ARRL    = 0x7D;  //  Low  byte for 16 MHz -> 8 kHz
        } // else if
        else
        {   // FREQ_1KHZ
            TIM2_ARRH    = 0x03;  //  High byte for 16 MHz -> 1 kHZ
            TIM2_ARRL    = 0xE8;  //  Low  byte for 16 MHz -> 1 kHz
        } // else
    } // else
    TIM2_IER_UIE = 1;     //  Enable the update interrupts
    TIM2_CR1_CEN = 1;     //  Finally enable the timer
} // setup_timer2()

/*-----------------------------------------------------------------------------
  Purpose  : This routine initialises all the GPIO pins of the STM8 uC.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void setup_gpio_ports(void)
{
    //-----------------------------
    // PORT A defines
    //-----------------------------
    //  RX1 & TX1 are initialized by the UART module
    PA_DDR     &=  ~ALL_KEYS; // Set all keys as inputs
    PA_CR1     |=   ALL_KEYS; // Enable pull-up resistors
    
    //-----------------------------
    // PORT B defines
    //-----------------------------
    PB_DDR     |=   ALL_LEDS; // Set as output
    PB_CR1     |=   ALL_LEDS; // Set to push-pull
    PB_ODR     &=  ~ALL_LEDS; // All leds 0 at power-up
    
    //-----------------------------
    // PORT C defines
    //-----------------------------
    PC_DDR     |=  (SDIN3 | SDIN2 | SDIN1 | SHCP | STCP); // Set as outputs
    PC_CR1     |=  (SDIN3 | SDIN2 | SDIN1 | SHCP | STCP); // Set to Push-Pull
    PC_ODR     &= ~(SDIN3 | SDIN2 | SDIN1 | SHCP | STCP); // Outputs are OFF
    PC_DDR     &= ~FREQ;  // Set frequency to input
    PC_CR1     |=  FREQ;  // Enable pull-up resistor
    EXTI_CR1_PCIS = 0x01; // PORTC External interrupt to Rising Edge only
    
    //-----------------------------
    // PORT D defines
    //-----------------------------
    // RX2 & TX2 are initialized by the UART module
    // SWIM is initialized by the JTAG module
    // BEEP function: set ST-LINK->Option Bytes->AFR7 to Alternate Active
    PD_DDR     |=  (CLK3 | DIO3 | CLK2 | DIO2 | IRQ_LED); // Set as outputs
    PD_CR1     |=  (CLK3 | DIO3 | CLK2 | DIO2 | IRQ_LED); // Set to Push-Pull
    PD_ODR     &= ~(CLK3 | DIO3 | CLK2 | DIO2 | IRQ_LED); // Outputs are OFF
    
    //-----------------------------
    // PORT E defines
    //-----------------------------
    PE_ODR     |=  (I2C_SCL | I2C_SDA); // Must be set here, or I2C will not work
    PE_DDR     |=  (I2C_SCL | I2C_SDA | BG_LED | VPK | VRMS | CLK4 | DIO4); // Set as outputs
    PE_CR1     |=  (I2C_SCL | I2C_SDA | BG_LED | VPK | VRMS | CLK4 | DIO4); // Set to push-pull
    
    //-----------------------------
    // PORT G defines
    //-----------------------------
    PG_DDR     |=  (CLK1 | DIO1); // Set as outputs
    PG_CR1     |=  (CLK1 | DIO1); // Set to push-pull
    PG_ODR     &= ~(CLK1 | DIO1); // disable leds at power-up
} // setup_gpio_ports()
