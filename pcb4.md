# PCB4 Engineering Design Files
The PCBs were created with Eagle v9.3.2.

![PCB4 Schematics](img/PCB4_uC_Schematics.png)<br>
*PCB4 microcontroller Control-Board Schematics*

The PCB itself is a double layer PCB with most components being SMD. Some details:
- CON1, CON2, CON3 and CON4 are screw terminal-blocks for the Seven Segment Displays (SSD).
- CON5 is also a screw terminal-block, but for the I2C LCD-Display.
- CON6 is the UART interface. Note that signal levels are 5 V.
- CON7 is the JTAG programming interface, that needs to be connected to an ST-Link V2 USB adapter (see below) in order to program the microcontroller.
- CON8 contains the input key connections.
- CON9 contains all LED connections for the front-panel.
- CON10, CON11 and CON12 are flat-cable connectors that interface with PCB1, PCB2 and PCB3 respectively.

![PCB4 Layout top-layer](img/PCB4_uC_PCB_Front.png)<br>
*PCB4 Top-layer Layout*

#Gerber-Files
Gerber-files will be placed here when they become available. These are the production-files that can be sent to a PCB-manufacturer. I typically use JLCPCB, but any PCB manufacturer can be used.

# Programming the microcontroller
To start with, you'd need one of those fancy coloured ST-Link V2 USB adapters (which are very cheap to find on ebay/Aliexpress). 
 
![st-link-v2](img/st_link_v2.png)<br>
*The ST-Link V2 USB adapter*

- Download and install the IAR development environment for STM8 from the IAR website. After having installed this, the drivers for the ST-Link USB adapter are also installed.
- Since the source-code is substantially larger than 8 Kbyte (limit for the free version), you need a valid license for IAR if you want to make code-changes.
- Open the workspace in IAR (File -> Open -> Workspace... -> THD_Analyzer.eww), then do a Project -> Rebuild All.
- Press Ctrl-D. This opens the debugger and transfers the code to the microcontroller. 
- Press Ctrl-Shift-D to Stop Debugging. Remove the wires, re-cycle power and you are good to go!



