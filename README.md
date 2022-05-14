# Bob Cordell's THD Analyzer
Bob Cordell created a series of articles in Audio Magazine, from July to September 1981 (!). In these articles, he described
in detail how to build a Total Harmonic Distortion (THD) Analyzer. I first heard of Bob Cordell, when I bought his book called "Designing Audio Power Amplifiers". 
I have never seen such an excellent written book about designing audio power amplifiers before, especially the first few chapters on continually improving a basic design 
to a high-end audio amplifier impressed me. I used the knowledge gained from his book as a base for teaching analog electronics for 3rd grade students Electrical Engineering.

I was also deeply impressed by the wealth of information in these articles and decided to build such a THD Analyzer myself. Bob
used some custom made switches with lots of decks to do the actual switching. Over the years I managed to find and construct these
switches, made some custom PCBs, created a nice front-end display and had my way of creating an implementation. As of today, this is
still work in progress.

Meanwhile, I decided to build another version of this THD Analyzer, which you will find on this page. It is the same
THD Analyzer, but now controlled by relays and a STM8S105C6 microcontroller. Such an implementation was discussed on the diyaudio forum
where someone called Giulianodes created another version where you would still need switches, but most switching was now done by relays.

His PCBs were very helpful in constructing this version of the THD Analyzer. Most ideas here are not mine, but originated from Bob Cordell
and from Giulianodes' PCB implementation.

I wanted to construct a THD Analyzer that was completely digital, while maintaining the original quality from Bob's design. 
I wanted 7-segment displays (SSD) for voltages, frequency and distortion (instead of an analog meter).
I also wanted to control all settings with keys and a LCD-display. I wanted an UART interface with commands, so I could do unit-testing of the boards.
And switching of all resistors and capacitors should also be done by relays.

In the end, I created five PCBs, one for each original PCB from Bob, one for the power-supply and one for the microcontroller itself. They are connected with flat-cable connectors.
Every PCB has the original analog design from Bob together with added relays. These relays are controlled by shift-registers, so off-board IO is very limited.
The microcontroller controls the shift-registers on every board and thus every single relay on these boards.

I decided to use the G6K-2F-Y relay for a few reasons: they are small, they are cheap and they are readily available (30 pcs for €26 on AliExpress). I also used the +12V
version, because current is limited (9.1 mA) and +12V can be made from the existing +15V DC. There are two switches present in every relay, which is very convenient.

# Features
The current PCB and firmware have the following features:
- Four 7-Segment-Displays displaying 1) Output Level, in RMS or peak voltage 2) Display of actual frequency, measured by the microcontroller 3) Distortion level in dB or as a percentage 4) Input Level in RMS or peak voltage
- LCD display displaying actual settings
- Five keys Up, Down, Left, Right and OK for
- Main output, Sync Output, Input and Distortion Output (same as in original design)
- Level Output potmeter (same as in original design)
- Four LEDs showing frequency too high/too low and input level too high/too low (same as in original design)
- All components are SMD where possible.

![Block Diagram](img/Blokschema_THD_Analyzer.png)<br>
*Block-Diagram THD-Analyzer*

# PCB 1: Signal Generator
This was the Signal Source PCB1 from Bob's original design. I made a couple of changes, compared to his original design:
- Switches S1J and S1H controlling the four trimpots are replaced by relays K17-K20 and are now on-board
- Switches S1I and S1G are replaced by relays K17-K20
- Switch S2 (A, B, C, D) are replaced by relays K14 and K21-K24
- Switching of R(F) and R(E) resistors is done by K1-K10
- Switching of C(KM) and C(LN) capacitors is done by K11-K13, the 200 kHz capacitors (200 pF with 68 Ohm in series) are always connected and need not be switched.

I named the bits from the HC595 shift-registers R01-R24 (R15 and R16 are not connected), with R01 being the first bit to shift into the shift-registers. They have the following meaning:
- R01: f=25 Hz to R10: f=200 Hz. For f=20 Hz all bits are 0.
- R11: Range = 200 Hz, R12: Range = 2 kHz, R13: Range is 20 kHz. For Range = 200 kHz these bits are all 0.
- R14: Output Level is 5 V
- R17: Range = 200 Hz, R18: Range = 2 kHz, R19: Range is 20 kHz, R20: Range is 200 kHz. Bits R17-R19 are duplicates of R11-R13 and could be removed, but we needed 3 shift-registers anyway (so no need to remove these).
- R21: Output Level if not OFF
- R22: Output Level is 0.15 V
- R23: Output Level is 0.5 V
- R24: Output Level is 1.5 V or 5 V

This PCB1 more or less copies the full-wave precision rectifier as found on PCB3. I wanted to have the MAIN.OUT signal from PCB1 measured in both amplitude and frequency. The amplitude is rectified to a DC-level and sent to ADC1, the
frequency is measured by comparing the positive rectified signal to half the DC-level and then send it to a timer-input (TIM1_CH1) for capture of the period-time.

Engineering design files (schematics, PCB-layout) are found on the [PCB1](./pcb1.md) page.

# PCB 2: Bandpass Filters and Product Amplifier and Level Set

# PCB 3: Amplitude and Frequency Detectors and Filter and Meter circuits

# PCB 4: Microcontrolller PCB

# PCB 5: Power-Supply

