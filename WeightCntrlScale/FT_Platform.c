/*
 * FT_Platform.c
 *
 *  Created on: Apr 4, 2017
 *      Author: mquaizar
 */


#include <stdint.h>

#include "FT_Platform.h"
#include "tm4c_spi2.h"
#include "tm4c_port.h"

#include "FreeRTOS\Source\include\FreeRTOS.h"
#include "FreeRTOS\Source\include\task.h"

void FT_Init(void)
{
	SPI2_Init();
	PORT_PB2out_Init();
	PORT_PB5out_Init();
}


void FT800_CS_SET(void)
{
	PORT_PB5out_Set();
}

void FT800_CS_CLEAR(void)
{
	PORT_PB5out_Clear();
}

void FT800_PD_N_SET(void)
{
	PORT_PB2out_Set();
}
void FT800_PD_N_CLEAR(void)
{
	PORT_PB2out_Clear();
}

uint8_t FT_spiRdWt(uint8_t data)
{
	return(SPI2_RdWt(data));
}

void FT_Delay(uint16_t ms)
{
	  portTickType ui32WakeTime = xTaskGetTickCount();
	  vTaskDelayUntil(&ui32WakeTime, ms / portTICK_RATE_MS);
}

