//*****************************************************************************
//
// maino.c - Main Entrace module;
//
//
//*****************************************************************************
#include <stdint.h>

#include "config_tm4c.h"

#include "task_display.h"
#include "task_weight.h"
#include "task_touch.h"
#include "task_pushbutton.h"

#include "osMutex_FT.h"

#include "task_third.h"

#include "FreeRTOS\Source\include\FreeRTOS.h"
#include "FreeRTOS\Source\include\task.h"
#include "FreeRTOS\Source\include\queue.h"
#include "FreeRTOS\Source\include\semphr.h"



//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************

int
main(void)
{

	//hardware configuration;
	CONFIG_Init();

    if(DISPLAY_TaskInit() != 0)
    {

        while(1)
        {
        }
    }

    if(TOUCH_TaskInit() != 0)
    {
      while(1)
      {
      }
    }

	if(PUSHBUTTON_TaskInit() != 0)
    {
        while(1)
        {
        }
    }


	if(WEIGHT_TaskInit() != 0)
    {

        while(1)
        {
        }
    }

    
    if(THIRD_TaskInit() != 0)
    {
      while(1)
      {
      }
    }

    /* Create a mutex type semaphore. */
    OSMutexFT_Init();

    //
    // Start the scheduler.  This should not return.
    //
    vTaskStartScheduler();

    //
    // In case the scheduler returns for some reason, print an error and loop
    // forever.
    //

    while(1)
    {
    }
}
