/*==================================================================
  File Name    : comms.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains functions for the uart commands.
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
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart.h"
#include "comms.h"
#include "i2c_bb.h"
#include "scheduler.h"
#include "eep.h"
#include "THD_Analyzer_main.h"

char rs232_inbuf[UART_BUFLEN]; // buffer for RS232 commands
uint8_t rs232_ptr = 0;         // index in RS232 buffer

extern char version[];
extern uint32_t pcb1_bits;    // 32 bits for PCB1 Shift-registers
extern uint32_t pcb2_bits;    // 32 bits for PCB2 Shift-registers
extern uint16_t pcb3_bits;    // 16 bits for PCB3 Shift-registers
extern int8_t  amplitude;     // [VRMS, VPEAK, VPP]

/*-----------------------------------------------------------------------------
  Purpose  : Scan all devices on the I2C bus
  Variables: -
 Returns  : -
  ---------------------------------------------------------------------------*/
void i2c_scan(void)
{
    char    s[50]; // needed for printing to serial terminal
    uint8_t x = 0;
    int     i;     // Leave this as an int!
    
    xputs("I2C: ");
    for (i = 0x00; i < 0xff; i+=2)
    {
        if (i2c_start_bb(i) == I2C_ACK)
        {
            sprintf(s,"0x%0x ",i);
            xputs(s);
            x++;
        } // if
        i2c_stop_bb();
    } // for
    if (!x) xputs("-");
    xputs("\n");
} // i2c_scan()

/*-----------------------------------------------------------------------------
  Purpose  : Non-blocking RS232 command-handler via the UART
  Variables: -
  Returns  : [NO_ERR, ERR_CMD, ERR_NUM, ERR_I2C]
  ---------------------------------------------------------------------------*/
uint8_t rs232_command_handler(void)
{
  uint8_t ch;
  static bool cmd_rcvd = 0;
  
  if (!cmd_rcvd && uart_kbhit())
  { // A new character has been received
    ch = tolower(uart_read()); // get character as lowercase
    uart_write(ch);
    switch (ch)
    {
        case '\r': break;
        case '\n': cmd_rcvd  = true;
                   rs232_inbuf[rs232_ptr] = '\0';
                   rs232_ptr = false;
                   break;
        default  : if (rs232_ptr < UART_BUFLEN-1)
                        rs232_inbuf[rs232_ptr++] = ch;
                   else rs232_ptr = 0; // remove inputs
                   break;
    } // switch
  } // if
  if (cmd_rcvd)
  {
    cmd_rcvd = false;
    return execute_single_command(rs232_inbuf);
  } // if
  else return NO_ERR;
} // rs232_command_handler()

/*-----------------------------------------------------------------------------
  Purpose  : separates a command into sub-strings, e.g. 'wb 1234 ff' or 'sp=120'
  Variables: -
  Returns  : number of substrings found in command string s
  ---------------------------------------------------------------------------*/
uint8_t process_string(char *s, char *s1, uint16_t *d1, uint16_t *d2)
{
    uint8_t i = 0;
    uint8_t len = strlen(s);
    
    s1[0] = '\0';
    *d1   = *d2 = 0;
    while ((i < len) && (s[i] != ' ') && (s[i] != '=')) i++;
    strncpy(s1,s,i); // copy command into 1st string
    s1[i] = '\0';    // terminate string
    if (i >= len) return 1; // only 1 item in command
    else if (s[i] == '=')
    {
        *d1 = (uint16_t)strtol(&s[i+1],NULL,10);
        return 2; // 2 items, return 2nd substring as a decimal number
    } // if
    *d1 = (uint16_t)strtol(&s[++i],NULL,16); // address in hex
    while ((i < len) && (s[i] != ' ')) i++;  // find next space
    if (i >= len) return 2;                  // no more data
    *d2 = (uint16_t)strtol(&s[i+1],NULL,16); // data in hex
    return 3;
} // process_string()

/*-----------------------------------------------------------------------------
  Purpose: interpret commands which are received via the UART:
   - O0/O1        : O0: select NTC temp. O1: select DS18B20 temp.
   - S0           : Display version number
     S1           : List all connected I2C devices  
     S2           : List all tasks
     S3           : Show DS18B20 temperature
  Variables: 
          s: the string that contains the command from UART
  Returns  : [NO_ERR, ERR_CMD, ERR_NUM, ERR_I2C] or ack. value for command
  ---------------------------------------------------------------------------*/
uint8_t execute_single_command(char *s)
{
   uint8_t  num    = atoi(&s[1]); // convert number in command (until space is found)
   uint8_t  rval   = NO_ERR;
   char     s2[30]; // Used for printing to uart
   char     s3[10]; // contains 1st sub-string of s
   uint16_t d1,d2;
   int8_t   rly;
   float    f;
   
   if (isalpha(s[1]))
   {   // 2-character command
       process_string(s,s3,&d1,&d2);
       if (!strcmp(s3,"rb"))
       {   // Read Byte
           sprintf(s2,"0x%X (%d)\n",*(uint8_t *)d1,*(uint8_t *)d1);
           xputs(s2);
       } // else if
       else if (!strcmp(s3,"rw"))
       {   // Read Word
           sprintf(s2,"%X (%d)\n",*(uint16_t *)d1,*(uint16_t *)d1);
           xputs(s2);
       } // else if
       else if (!strcmp(s3,"wb"))
       {   // Write Byte
           *(uint8_t *)d1 = (uint8_t)d2;
       } // else if
       else if (!strcmp(s3,"ww"))
       {   // Write Word
           *(uint16_t *)d1 = d2;
       } // else if
   } // else
   else 
   {  // single letter command
      switch (s[0])
      {
       case 'a': // Amplitude command
          switch (num)
          {
             case 0: amplitude = VRMS;
                     xputs("Set to Vrms\n");
                     break;
             case 1: amplitude = VPEAK;
                     xputs("Set to Vpeak\n");
                     break;
             case 2: amplitude = VPP;
                     xputs("Set to Vpp\n");
                     break;
            default: rval = ERR_NUM;
          } // switch
          break;
       case 'c': // Calibration constants in eeprom
          if (strlen(s) < 3) 
          {    // no number, so read it from eeprom
               f = eeprom_read_float(num<<2); // 1 float = 4 bytes
               sprintf(s2,"eep[%d]=%f\n",num,f);
               xputs(s2);
          } // if
          else 
          {
                if (num > 9) 
                     f = atof(&s[4]);
                else f = atof(&s[3]);
                eeprom_write_float(num<<2,f);
                sprintf(s2,"writing eep[%d] with %f\n",num,f);
                xputs(s2);
                eep_init(false); // update conversion parameters
          } // else
          break;
       case 'd': // Distortion Sensitivity command
          if (num <= SENS_10_000)
               set_sensitivity(num,SEND);
          else rval = ERR_NUM;
          break;
       case 'f': // Frequency command
          if (num <= FREQ_200_KHZ)
               set_frequency(num,SEND);
          else rval = ERR_NUM;
          break;
       case 'i': // Input Level command
          if (num <= INPUT_100V)
               set_input_level(num,SEND);
          else rval = ERR_NUM;
          break;
       case 'l': // Output Level command
          if (num <= LEVEL_5V)
               set_output_level(num,SEND);
          else rval = ERR_NUM;
          break;
      case 'r': // Relay switch-test
          if (strlen(s) < 3)
               rly = -1;
          else rly = atoi(&s[3]); // convert relay number
          switch (num)
          {
            case 1: // PCB1
              if (rly == 0)      pcb1_bits = 0L; // reset bits
              else if (rly < 0)
              {
                sprintf(s2,"R1 = 0x%08lX\n",pcb1_bits);
                xputs(s2);
              } // else
              else if (rly > 24) rval = ERR_NUM;
              else               pcb1_bits |= (1L << (rly-1)); // set 1 relay
              send_to_hc595(); // send to hardware
              break;
            case 2: // PCB2
              if (rly == 0)      pcb2_bits = 0L; // reset bits
              else if (rly < 0)
              {
                sprintf(s2,"R2 = 0x%08lX\n",pcb2_bits);
                xputs(s2);
              } // else
              else if (rly > 24) rval = ERR_NUM;
              else               pcb2_bits |= (1L << (rly-1)); // set 1 relay
              send_to_hc595(); // send to hardware
              break;
            case 3: // PCB3
              if (rly == 0)      pcb3_bits = 0; // reset bits
              else if (rly < 0)
              {
                sprintf(s2,"R3 = 0x%04X\n",pcb3_bits);
                xputs(s2);
              } // else
              else if (rly > 16) rval = ERR_NUM;
              else               pcb3_bits |= (1 << (rly-1)); // set 1 relay
              send_to_hc595(); // send to hardware
              break;
            default:
              rval = ERR_NUM;
              break;
          } // switch
          break;
       case 's': // System commands
               switch (num)
               {
               case 0: // Revision number
                   xputs(version);
                   break;
               case 1: // List all I2C devices
                   i2c_scan();
                   break;
               case 2: // List all tasks
                   list_all_tasks(); 
                   break;	
               default: rval = ERR_NUM;
                        break;
               } // switch
               break;
      default: rval = ERR_CMD;
	       break;
      } // switch
    } // else
   return rval;	
} // execute_single_command()
