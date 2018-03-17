/*
 * tm4c_port.h
 *
 *  Created on: Apr 4, 2017
 *      Author: mquaizar
 */

#ifndef TM4C_PORT_H_
#define TM4C_PORT_H_

#include <stdint.h>

//HX711 data clock;
void PORT_PA2out_Init(void);
void PORT_PA2out_Set(void);
void PORT_PA2out_Clear(void);
void PORT_PA2out_Toggle(void);

//Hx711 data In;
void PORT_PA3in_Init(void);
uint8_t PORT_PA3(void);

//FTDI Power down;
void PORT_PB2out_Init(void);
void PORT_PB2out_Set(void);
void PORT_PB2out_Clear(void);

//FTDI chip select
void PORT_PB5out_Init(void);
void PORT_PB5out_Set(void);
void PORT_PB5out_Clear(void);

//push button
void PORT_PE0in_Init(void);
uint8_t PORT_PE0(void);

//push button
void PORT_PE1in_Init(void);
uint8_t PORT_PE1(void);

//realy
void PORT_PE3out_Init(void);
void PORT_PE3out_Set(void);
void PORT_PE3out_Clear(void);
void PORT_PE3out_Toggle(void);

#endif /* TM4C_PORT_H_ */
