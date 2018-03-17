//*****************************************************************************
//
// task_third.c
//
//*****************************************************************************

#include <stdint.h>

#include "task_touch.h"
#include "touchscreen.h"

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

#define TOUCHSTASKSTACKSIZE        128         // Stack size in words
extern xQueueHandle FT_Semaphore;

static void TOUCH_Task(void *pvParameters);


uint32_t TOUCH_TaskInit(void)
{

    //
    // Create the LED task.
    //
    if(xTaskCreate(TOUCH_Task, (signed portCHAR *)"TOUCH", TOUCHSTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_TOUCH_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}

static void TOUCH_Task(void *pvParameters)
{

    while(1)
    {
    	TOUCHSCREEN_statemachine();
    }
}





