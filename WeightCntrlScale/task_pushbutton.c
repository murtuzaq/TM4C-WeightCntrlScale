//*****************************************************************************
//
// task_pushbutton.c
//
//*****************************************************************************

#include <stdint.h>

#include "task_pushbutton.h"
#include "button.h"

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

#define PUSHBUTTONSTASKSTACKSIZE        128         // Stack size in words


static void PUSHBUTTON_Task(void *pvParameters);


uint32_t PUSHBUTTON_TaskInit(void)
{

    //
    // Create the LED task.
    //
    if(xTaskCreate(PUSHBUTTON_Task, (signed portCHAR *)"PUSHBUTTON", PUSHBUTTONSTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_PUSHBUTTON_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}

static void PUSHBUTTON_Task(void *pvParameters)
{
	uint32_t k;
	BUTTON_Init();

    while(1)
    {
    	BUTTON_statemachine();

    	//
		//debounce and wait 100ms;
		//

    	vTaskDelay( 100 / portTICK_RATE_MS );
    	k++;
    }
}

void PUSHBUTTON_NotifyActive(void)
{

}




