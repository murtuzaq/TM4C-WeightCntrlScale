/*
 * osMutex_FT.c
 *
 *  Created on: Apr 10, 2017
 *      Author: mquaizar
 */

//mutuex to control the FT module

#include <stdint.h>

#include "FreeRTOS\Source\include\FreeRTOS.h"
#include "FreeRTOS\Source\include\task.h"
#include "FreeRTOS\Source\include\queue.h"
#include "FreeRTOS\Source\include\semphr.h"

xQueueHandle FT_Semaphore = NULL;

void OSMutexFT_Init(void)
{
    FT_Semaphore = xSemaphoreCreateMutex();

    if( FT_Semaphore != NULL )
    {
        /* The semaphore was created successfully and
        can be used. */
    }
}

//FT Mutex function takes in a void void function
void OSMutexFT_use(void (*ftfunc)(void))
{
	if( FT_Semaphore != NULL )
	{
        if( xSemaphoreTake( FT_Semaphore, portMAX_DELAY ) == pdTRUE )
        {
        	ftfunc();

        	xSemaphoreGive( FT_Semaphore );
            taskYIELD();
        }
	}
}

//FT Mutex function takes in a uint8_t void function
uint8_t OSMutexFT_u8use(uint8_t (*u8ftfunc)(void))
{
	uint8_t rvalue = 0; //return value;

	if( FT_Semaphore != NULL )
	{
        if( xSemaphoreTake( FT_Semaphore, portMAX_DELAY ) == pdTRUE )
        {
        	rvalue = u8ftfunc();

        	xSemaphoreGive( FT_Semaphore );
            taskYIELD();
        }
	}

	return(rvalue);
}
