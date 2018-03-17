/*
 * tm4c_port.c
 *
 *  Created on: Apr 4, 2017
 *      Author: mquaizar
 */

#include <stdint.h>
#include "tm4c_port.h"

#define SYSCTL_RCGCGPIO_R		(*((volatile uint32_t *)0x400FE608))

#define GPIO_PORTA_DATA_R		(*((volatile uint32_t *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_DEN_R		(*((volatile uint32_t *)0x4000451C))
#define GPIO_PORTA_PCTL_R		(*((volatile uint32_t *)0x4000452C))

#define GPIO_PORTB_DATA_R		(*((volatile uint32_t *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_DEN_R		(*((volatile uint32_t *)0x4000551C))

#define GPIO_PORTE_DATA_R		(*((volatile uint32_t *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_DEN_R		(*((volatile uint32_t *)0x4002451C))



void PORT_PA2out_Init(void)
{
  SYSCTL_RCGCGPIO_R |= (1 << 0U);  //Enable Clock

  GPIO_PORTA_PCTL_R &= ~(0xF << 8); //clear PORT Cntrl

  GPIO_PORTA_DIR_R |= (1 <<2U);    //Enable port as output

  GPIO_PORTA_DEN_R |= (1 <<2U);    //Enable port as Digital IO


}

void PORT_PA2out_Set(void)
{
	GPIO_PORTA_DATA_R |= (1 <<2U);
}

void PORT_PA2out_Clear(void)
{
	GPIO_PORTA_DATA_R &= ~(1 <<2U);
}

void PORT_PA2out_Toggle(void)
{
	GPIO_PORTA_DATA_R ^= (1 <<2U);
}

void PORT_PA3in_Init(void)
{
  SYSCTL_RCGCGPIO_R |= (1 << 0U);  //Enable Clock

  GPIO_PORTA_PCTL_R &= ~(0xF << 12); //clear PORT Cntrl

  GPIO_PORTA_DIR_R &= ~(1 <<3U);   //Enable port as input

  GPIO_PORTA_DEN_R |= (1 <<3U);    //Enable port as Digital IO

}

//read back the value of PA3
uint8_t PORT_PA3(void)
{
	if ((GPIO_PORTA_DATA_R & (1 << 3U)) == (1 <<3U))
		return(1);
	else
		return(0);

}


void PORT_PB2out_Init(void)
{
  SYSCTL_RCGCGPIO_R |= (1 << 1U);  //Enable Clock

  GPIO_PORTB_DIR_R |= (1 <<2U);    //Enable port as output

  GPIO_PORTB_DEN_R |= (1 <<2U);    //Enable port as Digital IO
}

void PORT_PB2out_Set(void)
{
	GPIO_PORTB_DATA_R |= (1 <<2U);
}

void PORT_PB2out_Clear(void)
{
	GPIO_PORTB_DATA_R &= ~(1 <<2U);
}

void PORT_PB5out_Init(void)
{
  SYSCTL_RCGCGPIO_R |= (1 << 1U);  //Enable Clock

  GPIO_PORTB_DIR_R |= (1 <<5U);    //Enable port as output

  GPIO_PORTB_DEN_R |= (1 <<5U);    //Enable port as Digital IO
}

void PORT_PB5out_Set(void)
{
	GPIO_PORTB_DATA_R |= (1 <<5U);
}

void PORT_PB5out_Clear(void)
{
	GPIO_PORTB_DATA_R &= ~(1 <<5U);
}


void PORT_PE0in_Init(void)
{
  SYSCTL_RCGCGPIO_R |= (1 << 4U);  //Enable Clock

  GPIO_PORTE_DIR_R &= ~(1 <<0U);   //Enable port as input

  GPIO_PORTE_DEN_R |= (1 <<0U);    //Enable port as Digital IO
}

//read back the value of PE0
uint8_t PORT_PE0(void)
{
	if ((GPIO_PORTE_DATA_R & (1 << 0U)) == (1 <<0U))
		return(1);
	else
		return(0);

}

void PORT_PE1in_Init(void)
{
  SYSCTL_RCGCGPIO_R |= (1 << 4U);  //Enable Clock

  GPIO_PORTE_DIR_R &= ~(1 <<1U);   //Enable port as input

  GPIO_PORTE_DEN_R |= (1 <<1U);    //Enable port as Digital IO
}

//read back the value of PE0
uint8_t PORT_PE1(void)
{
	if ((GPIO_PORTE_DATA_R & (1 << 1U)) == (1 <<1U))
		return(1);
	else
		return(0);

}


void PORT_PE3out_Init(void)
{
  SYSCTL_RCGCGPIO_R |= (1 << 4U);  //Enable Clock

  GPIO_PORTE_DIR_R |= (1 <<3U);    //Enable port as output

  GPIO_PORTE_DEN_R |= (1 <<3U);    //Enable port as Digital IO

  PORT_PE3out_Clear();
}

void PORT_PE3out_Set(void)
{
	GPIO_PORTE_DATA_R |= (1 <<3U);
}

void PORT_PE3out_Clear(void)
{
	GPIO_PORTE_DATA_R &= ~(1 <<3U);
}

void PORT_PE3out_Toggle(void)
{
	GPIO_PORTE_DATA_R ^= (1 <<3U);
}




