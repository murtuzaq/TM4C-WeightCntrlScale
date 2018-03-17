/*
 * tm4c_eeprom.h
 *
 *  Created on: Apr 12, 2017
 *      Author: mquaizar
 */

#ifndef TM4C_EEPROM_H_
#define TM4C_EEPROM_H_


void EEPROM_Init(void);
void EEPROM_Write(unsigned int data, unsigned int offset);
unsigned int EEPROM_Read(unsigned int offset);


#endif /* TM4C_EEPROM_H_ */
