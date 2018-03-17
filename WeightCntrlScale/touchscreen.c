/*
 * touchscreen.c
 *
 *  Created on: Apr 6, 2017
 *      Author: Murtuza
 */

#include <stdint.h>
#include "touchscreen.h"
#include "osMutex_FT.h"
#include "draw.h"
#include "FT_800.h"

static uint8_t TouchButton;

uint8_t TOUCHSCREEN_Read(void)
{
	uint8_t btn;

	btn = OSMutexFT_u8use(ReadPushButton);

	if (btn == 0 || btn == 0xFF)
	{
		btn = 0;
	}
	else
	{
		//do nothing;
		//pass forward the value from ReadPushButton;
	}

	return(btn);
}

void TOUCHSCREEN_statemachine(void)
{
	static uint8_t TOUCHSCREEN_State = 0;

	switch(TOUCHSCREEN_State)
	{
	case 0:
		TouchButton = TOUCHSCREEN_Read();
		if (!(TouchButton == 0))
		{

			TOUCHSCREEN_State++;
		}
		else
		{
			//do nothing but wait;
		}
		break;

	case 1:
		//wait for this button to be unlatched in the view application;
		while(TouchButton != 0){};

		TOUCHSCREEN_State = 0;

	default:
		break;
	}

}

//API To get Button Status;
void TOUCHSCREEN_ClearState(void)
{
	TouchButton = 0;
}

uint8_t TOUCHSCREEN_IsState(void)
{
	return(TouchButton);
}


