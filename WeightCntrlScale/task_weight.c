//*****************************************************************************
//
// task_weight.c
//
//*****************************************************************************

#include "task_weight.h"
#include "weight.h"
#include "tm4c_port.h"

#include <stdint.h>
#include "tm4c_adc1.h"

#include "priorities.h"
#include "FreeRTOS\Source\include\FreeRTOS.h"
#include "FreeRTOS\Source\include\task.h"
#include "FreeRTOS\Source\include\queue.h"
#include "FreeRTOS\Source\include\semphr.h"
//*****************************************************************************
//
// The stack size for the task.
//
//*****************************************************************************

#define WEIGHTTASKSTACKSIZE        128         // Stack size in words


static void WEIGHT_Task(void *pvParameters)
{
	uint32_t j;

	WEIGHT_Init();

	while(1)
    {
		WEIGHT_Measure();
		portTickType ui32WakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&ui32WakeTime, 100 / portTICK_RATE_MS);

		j++;
    }
}


uint32_t WEIGHT_TaskInit(void)
{

    //
    // Create the LED task.
    //
    if(xTaskCreate(WEIGHT_Task, (signed portCHAR *)"WEIGHT", WEIGHTTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_WEIGHT_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}



