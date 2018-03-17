/*
 * tm4c_eeprom.c
 *
 *  Created on: Apr 12, 2017
 *      Author: mquaizar
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"

void EEPROM_Init(void)
{
	uint32_t i;
     //pg 8.2.4.2;  539
   //EPROM Initialization and Configuration
   //Before writing to any EEPROM registers, the clock to the EEPROM module
   //must be enabled through the EEPROM Run Mode Clock Gating Control
   //(RCGCEEPROM) register (see page 356) and the following initialization

    SYSCTL_RCGCEEPROM_R  = 0x00000001;
   //steps must be executed:
   //1. Insert delay (6 cycles plus function call overhead).
    for ( i = 0; i < 0xFF; i++); //added a bit more delay since its startup;

   //2. Poll the WORKING bit in the EEPROM Done Status (EEDONE) register
   // until it is clear, indicating that the EEPROM has completed its power-on
   //initialization. When WORKING =0, continue.

    while(EEPROM_EEDONE_R & 0x01 == 0x01){}

   //3. Read the PRETRY and ERETRY bits in the EEPROM Support Control and
   //Status (EESUPP) register. If either of the bits are set, return an error,
    if ( ((EEPROM_EESUPP_R & 0x08) == 0x08) || ((EEPROM_EESUPP_R & 0x04) == 0x04))
    {
      while(1); //DIE HERE
    }
    //else continue.

   //4. Reset the EEPROM module using the EEPROM Software Reset (SREEPROM)
   //register at offset 0x558 in the System Control register space.
    SYSCTL_SREEPROM_R = 0x01;
    for (i = 0; i < 0xFF; i++);
    SYSCTL_SREEPROM_R = 0x00;

   //5. Insert delay (6 cycles plus function call overhead).
    for (i = 0; i < 0xFF; i++);

   //6. Poll the WORKING bit in the EEPROM Done Status (EEDONE) register to
   //determine when it is clear. When WORKING =0, continue.
    while(EEPROM_EEDONE_R & 0x01 == 0x01){}

   //7. Read the PRETRY and ERETRY bits in the EESUPP register. If either
   // of the bits are set, return an error, else the EEPROM initialization
   //is complete and software may use the peripheral as normal.

    if ( ((EEPROM_EESUPP_R & 0x08) == 0x08) || ((EEPROM_EESUPP_R & 0x04) == 0x04))
    {
      while(1); //DIE HERE
    }

    //if not dead, then select block 0 for EEPROM write.
    EEPROM_EEBLOCK_R  = 0x01;
    //set current offset of block to 0;
    EEPROM_EEOFFSET_R = 0;

    while(EEPROM_EEDONE_R & 0x01 == 0x01){}
}

void EEPROM_Write(unsigned int data, unsigned int offset)
{
  EEPROM_EEOFFSET_R = offset;
  EEPROM_EERDWR_R = data;

  while(EEPROM_EEDONE_R & 0x01 == 0x01){}
}

unsigned int EEPROM_Read(unsigned int offset)
{
  EEPROM_EEOFFSET_R = offset;
  return(EEPROM_EERDWR_R);
}
