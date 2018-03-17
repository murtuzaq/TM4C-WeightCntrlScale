/*
 * hx711.c
 *
 *  Created on: May 1, 2017
 *      Author: mquaizar
 */

#include <stdint.h>
#include "tm4c_port.h"


void HX711_Init(void)
{
	PORT_PA2out_Init();
	PORT_PA3in_Init();

	PORT_PA2out_Clear();
}

uint32_t HX711_Run(void)
{
	uint32_t Count;
	uint8_t i;
	uint8_t noOperationCnt;

	Count = 0;
	//wait for Data Line to go low;
	while(PORT_PA3() == 1) {};

	for(i = 0; i < 25; i++)
	{
		PORT_PA2out_Set();

		Count = Count << 1;
		noOperationCnt = 0;
		while(noOperationCnt++ < 5);

		noOperationCnt = 0;
		PORT_PA2out_Clear();
		while(noOperationCnt++ < 10);

		if(PORT_PA3() == 1)
			Count++;

	}

	//PORT_PA2out_Set();
	//Count = Count^0x00800000;
	//PORT_PA2out_Clear();

	return(Count);

}

