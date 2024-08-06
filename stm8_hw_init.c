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
extern uint16_t freq_meas;   // The measured frequency
extern uint8_t  fmt_meas;    // Format for freq_meas

uint16_t tmr1_ticks;         // sine wave period time in usec.
uint16_t tmr1;               // Timer1 value
uint16_t prev_tmr1;          // previous value of tmr1
int16_t  tim1_read_time = 0; // Time interval in msec. for reading timer 1
int16_t  tim1_read_cntr = 0; // Counter for reading timer 1
uint16_t buttons;            // Previous and Actual value of press-buttons
uint16_t minct;              // Min. number of clock-ticks for 1 full FREQ period

// Min. number of clock-ticks for a valid period of FREQ, 75% of default value
const uint16_t tmin[11] = {15000,12000,10000,7500,6000,4615,3750,3000,2307,1875,1500};

/*------------------------------------------------------------------
  Purpose  : This function is called from freq_task() every 200 msec.
             and calculates the actual frequency of the FREQ signal 
             on PC1/TIM1_CH1 and stores the result in freq_meas.
  Variables: f: the frequency set for the sine-wave generator
             tmr1_ticks: the measured clock_ticks difference of timer 1.
  Returns  : -
  ------------------------------------------------------------------*/
void calc_freq(void)
{
    uint16_t x; // contains tmr1_ticks difference
    
    freq_meas = 0; // init. to 0 in case of invalid measurement 
    if (freq_sine > FREQ_2000_HZ)
    {  // Frequency measurement, high-frequency, freq > 2 kHz
         __disable_interrupt();
         x = tmr1_ticks; // needed because tmr1_ticks is updated in an interrupt
         __enable_interrupt();
         switch (freq_sine)
         {
             case FREQ_2500_HZ:               // 400 msec. => 1000 clock-ticks
             case FREQ_3000_HZ:               // 400 msec. => 1200 clock-ticks
             case FREQ_4000_HZ:               // 400 msec. => 1600 clock-ticks
                  if ((x > CLK_TICKS_MIN) && (x < CLK_TICKS_MAX))
                  {
                    freq_meas = (x << 2) + x; // freq_meas = 5 * tmr1_ticks
                    freq_meas ++;             // one for rounding
                    freq_meas >>= 1;          // freq_meas = 2.5 * tmr1_ticks
                    fmt_meas = DP0_HZ;        // no decimals, value in Hz
                  } // if
                  break;
             case FREQ_5000_HZ:               // 200 msec. => 1000 clock-ticks
             case FREQ_6500_HZ:               // 200 msec. => 1300 clock-ticks
             case FREQ_8000_HZ:               // 200 msec. => 1600 clock-ticks
                  if ((x > CLK_TICKS_MIN) && (x < CLK_TICKS_MAX))
                  {
                    freq_meas = (x << 2) + x; // freq_meas = 5 * tmr1_ticks
                    fmt_meas = DP0_HZ;        // no decimals, value in Hz
                  } // if
                  break;
             case FREQ_10_KHZ:                // 100 msec. => 1000 clock-ticks
                  if ((x > CLK_TICKS_MIN) && (x < CLK_TICKS_MAX))
                  {
                    if (x >= 1000)            // display in Hz or kHz?
                    {
                       freq_meas = x;         // no scaling needed
                       fmt_meas  = DP2_KHZ;   // 2 decimals, value in kHz, '10.23'
                    } // if
                    else 
                    { 
                       freq_meas = (x<<3) + (x<<1); // freq_meas = 10 * tmr1_ticks
                       fmt_meas = DP0_HZ;           // no decimals, value in Hz, '995'
                    } // else
                  } // if
                  break;
             case FREQ_100_KHZ:                  // 100 msec. => 10.000 clock-ticks
                  if ((x > CLK_TICKS_100K_MIN) && (x < CLK_TICKS_100K_MAX))
                  {
                    if (x >= 10000)              // display kHz with 1 or 2 decimals?
                    {
                       freq_meas = divu10(x);    // divide by 10 to get display value
                       fmt_meas  = DP1_KHZ;      // 1 decimal, value in kHz, '100.5'
                    } // if
                    else 
                    {
                       freq_meas = x;            // no scaling
                       fmt_meas  = DP2_KHZ;      // 2 decimals, value in kHz, '99.95'
                    } // else
                  } // if
                  break;
             default:                            // read every 100 msec.
                  // 100 msec.  10 kHz => 1000   clock-ticks, display: 10.00 kHz
                  // 100 msec. 200 kHz => 20.000 clock-ticks, display: 200.0 kHz
                  if (freq_sine < FREQ_100_KHZ)
                  {    // 13, 16, 20, 25, 30,40, 50, 65, 80 kHz ; 1300..8000 clock-ticks
                       if ((x > CLK_TICKS_13K_80K_MIN) && (x < CLK_TICKS_13K_80K_MAX))
                       {
                          freq_meas = x;       // no scaling needed    
                          fmt_meas  = DP2_KHZ; // 2 decimals, value in kHz
                       } // if
                  } // if
                  else 
                  {    // 130, 160, 200 kHz ; 13.000..20.000 clock-ticks 
                       if ((x > CLK_TICKS_130K_200K_MIN) && (x < CLK_TICKS_130K_200K_MAX))
                       {
                          freq_meas = divu10(x); // 130 kHz => 13.000 ticks, 130.0 on display
                          fmt_meas  = DP1_KHZ;   // 1 decimal, value in kHz
                       } // if
                  } // else
                  break;
         } // switch
    } // if
    else
    { // freq_sine <= FREQ_2000_HZ
      __disable_interrupt();
      x = tmr1_ticks; // needed because tmr1_ticks is updated in an interrupt
      __enable_interrupt();
      if (freq_sine <= FREQ_200_HZ)
      {   // Period measurement, low-frequency, freq <= 200 Hz, Timer 1 clock 400 kHz
          // 20 Hz => 20.000 clock-ticks; 200 Hz => 2.000 clock-ticks
          fmt_meas = DP0_HZ; // no decimals, value in Hz
          if (x > CLK_PERIOD_MEAS_MIN)
          {
              freq_meas = (uint16_t)(CLK_400_KHZ / x); // 400 kHz clock
          } // if
      } // else if
      else
      {   // Period measurement, freq > 200 Hz && freq <= 2 kHz, Timer 1 clock 4 MHz
          // 250 Hz => 16.000 clock-ticks; 2 kHz => 2.000 clock-ticks
          fmt_meas = DP0_HZ; // no decimals, value in Hz
          if (x > CLK_PERIOD_MEAS_MIN)
          {
              freq_meas = (uint16_t)(CLK_4_MHZ / x); // 4 MHz clock
          } // if
      } // else
    } // else
} // calc_freq()

/*------------------------------------------------------------------
  Purpose  : This function reads Timer 1 and calculates the
             difference from the previous value. It is an inline
             function, since it is called from two interrupt routines:
             PORT_EXT_IRQ and TIM2_UPD_OVF.
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
static inline void read_timer1_delta_ticks(void)
{
    volatile uint16_t a;
    volatile uint16_t t = tmr1_val();
  
    if (t < tmr1)
         a = 65535 - tmr1 + t; 
    else a = t - tmr1;
    if (a > minct)
    {   // ignore false transients and half-period crossings
        prev_tmr1  = tmr1;
        tmr1       = t;
        if (tmr1 < prev_tmr1)
             tmr1_ticks = (uint16_t)(65535L + tmr1 - prev_tmr1);
        else tmr1_ticks = tmr1 - prev_tmr1;
    } // if
} // read_timer1_delta_ticks()

/*------------------------------------------------------------------
  Purpose  : This is the PORTC external interrupt routine. It is
             executed on every rising edge of PC1 when in 
             low-frequency period measurement.
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
#if defined(__ICCSTM8__)                        
   #if defined(USE_IAR_HEADER_FILE) 
       #pragma vector = EXTI2_vector
   #else
       #pragma vector = EXTI2_vector+2 /* IAR vectors have +2 offset */
   #endif
  __interrupt void PORTC_EXT_IRQ(void)
#elif defined(__SDCC)                   
    void PORTC_EXT_IRQ(void) __interrupt(EXTI2_vector)
#else 
  #error "Unsupported Compiler!"
#endif
{
    read_timer1_delta_ticks(); // Read Timer 1 difference
} // PORTC_EXT_IRQ()

/*------------------------------------------------------------------
  Purpose  : This is the Timer-interrupt routine for the Timer 2 
             Overflow handler which runs every msec. (f=1 kHz). 
             It is used by the task-scheduler.
             Max. duty-cycle measured (ISR pin): 7.18 %
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
#if defined(__ICCSTM8__)   
   #if defined(USE_IAR_HEADER_FILE) 
       #pragma vector = TIM2_OVR_UIF_vector
   #else
       #pragma vector = TIM2_OVR_UIF_vector+2 /* IAR vectors have +2 offset */
   #endif
__interrupt void TIM2_UPD_OVF_IRQHandler(void)
#elif defined(__SDCC)                   
void TIM2_UPD_OVF_IRQHandler(void) __interrupt(TIM2_OVR_UIF_vector)
#else 
  #error "Unsupported Compiler!"
#endif
{
    IRQ_LEDb = 1;      // Start Time-measurement
    t2_millis++;       // update millisecond counter
    scheduler_isr();   // call the ISR routine for the task-scheduler
    
    if (tim1_read_time > 0) // set by setup_timer1() function
    {   // frequency measurement of high-frequency signal
        if (++tim1_read_cntr >= tim1_read_time)
        {   // Time-out
            read_timer1_delta_ticks(); // Read Timer 1 difference
            tim1_read_cntr = 0;        // Restart timer
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
    TIM1_CR1_CEN   = 0;     // Disable the counter
    PC_CR2_FREQ    = 0;     // Disable interrupt for FREQ input pin
    __disable_interrupt();  // Disable interrupts
    tim1_read_time = 0;     // Disable reading TIM1 in interrupt
    if (f > FREQ_2000_HZ)
    {	// Frequency measurement, high-frequency
        
        // Par. 17.4.3 External clock source mode 1, TIM1 counts on each 
        // rising edge on timer input 1 (TIM1_CH1).
        TIM1_CCER1_CC1E = 0; // Make sure CC1S bits can be written
        TIM1_CCMR1_CC1S = 1; // Configure ch.1 to detect rising edges on TI1 input
        TIM1_CCMR1  &= 0x0F; // IC1F=0, no input filter duration, fsampling=fmaster
        TIM1_CCER1_CC1P = 0; // Capture on rising edge of TIM1_CH1
        TIM1_SMCR_SMS   = 7; // External clock mode 1, rising edges of trigger (TRGI) clock the counter
        TIM1_SMCR_TS    = 5; // Trigger input (TRGI) is set to filtered timer input 1 (TI1FP1)
        TIM1_PSCRH      = 0; // No divide, since signal is one-half rectified
        TIM1_PSCRL      = 0; 
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
        minct = 750; // 75% of min. number of clock-ticks
        //read_timer1_delta_ticks(); // Read Timer 1 difference as start-value
        //tim1_read_cntr = 0;        // start counting in TIM2_UPD_OVF_IRQHandler()
    } // if
    else // f <= FREQ_2000_HZ
    {   // TMEAS, Period measurement, low-frequency
        TIM1_SMCR_SMS = 0; // Par. 17.4.2, internal clock source (fmaster)
        TIM1_ETR_ECE  = 0; // If CEN=1, prescaler is clocked by the internal clock
        if (f <= FREQ_200_HZ)
        {   // Timer 1 clock 400 kHz: 20 Hz -> 20.000 ticks, 200 Hz -> 2.000 ticks
            TIM1_PSCRH = 0x0000; // 16 MHz / (39+1) = 400 kHz 
            TIM1_PSCRL = 0x0027; // 39 = 0x27
        } //
        else // f > 200 Hz && f <= 2 kHz
        {   // Timer 1 clock 4 MHz: 250 Hz -> 16.000 ticks, 2 kHz -> 2.000 ticks
            TIM1_PSCRH = 0x0000; // 16 MHz / (3+1) = 4 MHz 
            TIM1_PSCRL = 0x0003; // 3 = 0x03	
        } // else
        minct = tmin[freq_sine%11]; // get min number of clock-ticks for a full period
        PC_CR2_FREQ = 1;            // Enable interrupt for FREQ input pin on TIM1_CH1
    } // else
    TIM1_CR1_CEN = 1;     // Enable TIM1 counter
    __enable_interrupt(); // Enable interrupts again
} // setup_timer1()

/*-----------------------------------------------------------------------------
  Purpose  : This routine initialises the system clock to run at 16 MHz.
             It uses the internal HSI oscillator.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void initialise_system_clock(void)
{
    CLK_ICKR       = 0;           //  Reset the Internal Clock Register.
    CLK_ICKR_HSIEN = 1;           //  Enable the HSI.
    while (CLK_ICKR_HSIRDY == 0); //  Wait for the HSI to be ready for use.
    CLK_CKDIVR     = 0;           //  Ensure the clocks are running at full speed.
 
    // The datasheet lists that the max. ADC clock is equal to 6 MHz (4 MHz when on 3.3V).
    // Because fMASTER is now at 16 MHz, we need to set the ADC-prescaler to 4.
    ADC_CR1_SPSEL = 0x02;         //  Set prescaler (SPSEL bits) to 4, fADC = 4 MHz
    CLK_SWIMCCR   = 0x00;         //  Set SWIM to run at clock / 2.
    CLK_SWR       = 0xE1;         //  Use HSI as the clock source.
    CLK_SWCR      = 0;            //  Reset the clock switch control register.
    CLK_SWCR_SWEN = 1;            //  Enable switching.
    while (CLK_SWCR_SWBSY != 0);  //  Pause while the clock switch is busy.
} // initialise_system_clock()

/*-----------------------------------------------------------------------------
  Purpose  : This routine initialises Timer 2 to generate a 1, 2, 4 or 8 kHz interrupt.
  Variables: freq: [FREQ_1KHZ,FREQ_2KHZ,FREQ_4KHz,FREQ_8KHz]
  Returns  : -
  ---------------------------------------------------------------------------*/
void setup_timer2(uint8_t freq)
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
    TIM2_IER_UIE = 1;     //  Enable the update interrupts
    TIM2_CR1_CEN = 1;     //  Finally enable the timer
} // setup_timer2()

/*-----------------------------------------------------------------------------
  Purpose  : This routine reads the push-buttons UP, DOWN, LEFT, RIGHT and OK.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void read_buttons(void)
{
    uint8_t x = (PA_IDR & ALL_KEYS) ; // Buttons are in PA6..PA2
    
    x ^= ALL_KEYS;       // Invert keys, 0 = pressed
    buttons <<= 8;       // Shift older values of push-buttons
    buttons |= (x >> 2); // New value of push-buttons now in bits 4..0
} // read_buttons()

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
    PC_CR1     &= ~FREQ;  // Disable pull-up resistor
    PC_CR2_FREQ = 1;      // Enable interrupt
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
    PE_DDR     |=  (I2C_SCL | I2C_SDA | BG_LED | VPK_LED | VRMS_LED | CLK4 | DIO4); // Set as outputs
    PE_CR1     |=  (I2C_SCL | I2C_SDA | BG_LED | VPK_LED | VRMS_LED | CLK4 | DIO4); // Set to push-pull
    
    //-----------------------------
    // PORT G defines
    //-----------------------------
    PG_DDR     |=  (CLK1 | DIO1); // Set as outputs
    PG_CR1     |=  (CLK1 | DIO1); // Set to push-pull
    PG_ODR     &= ~(CLK1 | DIO1); // disable leds at power-up
} // setup_gpio_ports()
