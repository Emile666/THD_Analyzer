# SSD modules for displays and key scan
![SSD module PCB front layer](img/SSD_front.png)<br>
*SSD PCB front layer*

![SSD module PCB front layer](img/SSD_back.png)<br>
*SSD PCB back layer*

The PCB was created using KiCad v8.0. It is a double sided PCB with a 4 digit 7-segment LED display at the front and TM1637 IC at the back. Groundplane is used to minimize EMI transmission from the board. The connectors J1 and J2 can be mounted at either side, depending on wich is most convenient. Same for the 470uF cap, using av low voltage radial cap laying down, it will be even lower than a SMD cap.

# SSD module Schematic
[SSD module schematic](img/SSD_schematic.pdf)<br>
*SSD module schematic*

# Design-details
In the beginning of this project it was discovered that the SSD modules caused switching spikes at the +5V rail from the multiplexing of the LED displays. These voltages spikes did not only cause the frequency counter to malfunction, but also increased the residual distortion. Making the whole isntrument useless.

The orginal SSD modules from Ali-Express did came with only 2uF decoupling for the TM1637 ic, datasheet suggests 100uF. But even that is way too low. Instead a LC-filter was added to each SSD module, with good results. In order to minimze EMI being transimitted from the board or its power cables, the filter should be placed as close to the TM1637 as possible. A whole new PCB was then design for this purpose.

Component values of the LC filter are not very critical, the values there determined by experient. The series resistor improves the perfomance significantly.

Indicating LED for hz, khz etc. are now included at the board (LED A, B and C) and controlled by the TM1637 as well. Not that some board SSD1 and 2 only uses to LED's A and C. Do not fit B at these boards.

Since the TM1637 also features a key scan feature, connector J2 was added in orded connect switches (described at PCB4 page).

# Gerbers
This zip-file contains gerbers to be sent for manufacturing:  [SSD Gerber Files](img/SSD-gerbers.zip)
