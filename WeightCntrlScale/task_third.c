//*****************************************************************************
//
// task_third.c
//
//*****************************************************************************

#include <stdint.h>

#include "task_third.h"

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

#define THIRDSTASKSTACKSIZE        128         // Stack size in words


static void THIRD_Task(void *pvParameters);


uint32_t THIRD_TaskInit(void)
{

    //
    // Create the LED task.
    //
    if(xTaskCreate(THIRD_Task, (signed portCHAR *)"THIRD", THIRDSTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_THIRD_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}

static void THIRD_Task(void *pvParameters)
{
	uint32_t k;

    while(1)
    {
    	k++;
    }
}





