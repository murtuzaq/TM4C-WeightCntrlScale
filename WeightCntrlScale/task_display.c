//*****************************************************************************
//
// task_first.c
//
//*****************************************************************************

#include <stdint.h>

#include "priorities.h"
#include "FreeRTOS\Source\include\FreeRTOS.h"
#include "FreeRTOS\Source\include\task.h"
#include "FreeRTOS\Source\include\queue.h"
#include "FreeRTOS\Source\include\semphr.h"

#include "task_display.h"

#include "FT_800.h"
#include "view.h"
#include "draw.h"
//*****************************************************************************
//
// The stack size for the task.
//
//*****************************************************************************

#define DISPLAYSTASKSTACKSIZE        256         // Stack size in words
extern xQueueHandle FT_Semaphore;

static void DISPLAY_Task(void *pvParameters);


uint32_t DISPLAY_TaskInit(void)
{

    //
    // Create the task.
    //
    if(xTaskCreate(DISPLAY_Task, (signed portCHAR *)"DISPLAY", DISPLAYSTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_DISPLAY_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}



static void DISPLAY_Task(void *pvParameters)
{
	//initilize display;
	VIEW_SetImage(image_init);

    while(1)
    {
    	//run
    	VIEW_ImageState();
    }
}







