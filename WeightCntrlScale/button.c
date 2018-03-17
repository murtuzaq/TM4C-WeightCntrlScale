/*
 * button.c
 *
 *  Created on: Apr 6, 2017
 *      Author: Murtuza
 */

#include <stdint.h>
#include "tm4c_port.h"
#include "button.h"
#include "task_pushbutton.h"

static BUTTON_STATE btnState;

void BUTTON_Init(void)
{
	//PORT_PE0in_Init(); //zapped; broken
	PORT_PE1in_Init();
}

BUTTON_STATE BUTTON_Read(void)
{
	if (PORT_PE1() == 1)
	{
		return(BUTTON_RELEASE);
	}
	else
	{
		return(BUTTON_ACTIVE);
	}
}

void BUTTON_statemachine(void)
{
	static uint8_t Button_State = 0;

	switch(Button_State)
	{
	case 0:
		//catch the first edge trigger;
		if (BUTTON_Read() == BUTTON_ACTIVE)
		{
			Button_State++;
		}
		else
		{
			//do nothing but wait;
		}
		break;

	case 1:
		//check again if Button is acttive (100ms debounce time)
		if (BUTTON_Read() == BUTTON_ACTIVE)
		{
			//send out API that button has been pressed;
			//notify task that pushbutton has been pressed;
			//go to sleep until the notified task receives the
			//message and unblocks you.
			btnState = BUTTON_ACTIVE;
			while(btnState == BUTTON_ACTIVE){}

			//view task has cleared btnState,
			//wait for actural button to be released;
			while(BUTTON_Read() == BUTTON_ACTIVE){}
			Button_State = 0;
		}

		Button_State = 0;

	default:
		break;
	}

}

//API To get Button Status;
void BUTTON_ClearState(void)
{
	btnState = BUTTON_RELEASE;
}

BUTTON_STATE BUTTON_IsState(void)
{
	return(btnState);
}


