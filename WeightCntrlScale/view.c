/*
 * view.c
 *
 *  Created on: Apr 4, 2017
 *      Author: mquaizar
 */

#include <stdint.h>
#include "view.h"
#include "task_display.h"
#include "draw.h"
#include "button.h"
#include "weight.h"
#include "osMutex_FT.h"
#include "touchscreen.h"
#include "relay.h"

static ScreenImage view_image;
static uint32_t num_lbs;
static uint32_t num_ounces;
static uint32_t *numSelect;
static uint8_t digit;
static uint32_t view_weight;



void VIEW_ImageState(void)
{
	switch(view_image)
	{
	case image_init:
		OSMutexFT_use(DRAW_Init);
		RELAY_Off();
		BUTTON_ClearState();
		TOUCHSCREEN_ClearState();
		WEIGHT_GetLimitFromEEPROM();
		WEIGHT_GetScaleMultiplierFromEEPROM();
		WEIGHT_GetScaleZeroFromEEPROM();
		VIEW_SetImage(image_stg1Wght);
		break;

	case image_stg1Wght:
		OSMutexFT_use(DRAW_Stg1Wght);

		if (BUTTON_IsState() == BUTTON_ACTIVE)
		{
			BUTTON_ClearState();
			RELAY_On();
			VIEW_SetImage(image_stg2Wght);
		}
		else
		{
			switch(TOUCHSCREEN_IsState())
			{

			case BUTTON_START:
				RELAY_On();
				VIEW_SetImage(image_stg2Wght);
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_CALI:
				VIEW_SetImage(image_calibrStart);
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_LIMIT:
				VIEW_SetImage(image_setlimit);

				num_lbs = WEIGHT_GetLimit()/16;
				num_ounces = WEIGHT_GetLimit()%16;

				numSelect = &num_lbs;
				digit = 0;

				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_ABOUT:
				VIEW_SetImage(image_about);
				TOUCHSCREEN_ClearState();
				break;
			}
		}

		break;

	case image_stg2Wght:
		OSMutexFT_use(DRAW_Stg2Wght);

		view_weight = WEIGHT_GetValue();

		if (view_weight >= WEIGHT_GetLimit())
		{
			RELAY_Off();
		}

		if (IsRelayActive() == 0)
		{
			if (view_weight < (WEIGHT_GetLimit()/2) )
			{
				//object is detected to be removed from scale;
				VIEW_SetImage(image_stg1Wght);
			}
		}

		if (BUTTON_IsState() == BUTTON_ACTIVE)
		{
			BUTTON_ClearState();
			RELAY_Off();
			VIEW_SetImage(image_stg1Wght);
		}

		switch(TOUCHSCREEN_IsState())
		{

			case BUTTON_BACK:
				RELAY_Off();
				VIEW_SetImage(image_stg1Wght);
				TOUCHSCREEN_ClearState();
				break;

			default:
				break;
		}


		break;

/*
	case image_setlimit:
		OSMutexFT_use(DRAW_SetLimit);

		switch(TOUCHSCREEN_IsState())
		{
			case BUTTON_SETBIT2:
				LIMIT_IncHUNDdigit();
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_SETBIT1:
				LIMIT_IncTENSdigit();
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_SETBIT0:
				LIMIT_IncONESdigit();
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_RESET:
				LIMIT_Resetdigits();
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_BACK:
				VIEW_SetImage(image_stg1Wght);
				LIMIT_SaveToEEPROM();
				TOUCHSCREEN_ClearState();
				break;
		}

		break;
		*/

	case image_setlimit:

		OSMutexFT_use(DRAW_SetLimit);

		WeightKeypad();

		switch(TOUCHSCREEN_IsState())
		{


			case BUTTON_CANCEL:
				VIEW_SetImage(image_stg1Wght);
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_ENTER:
				VIEW_SetImage(image_stg1Wght);
				if (WEIGHT_SetLimit(num_lbs*16 + num_ounces) == WEIGHT_SUCCESS)
				{
					VIEW_SetImage(image_EndSuccess);
				}
				else
				{
					VIEW_SetImage(image_EndError);
				}
				TOUCHSCREEN_ClearState();
				break;
		}
		break;

	case image_calibrStart:
		OSMutexFT_use(DRAW_CaliStart);

		switch(TOUCHSCREEN_IsState())
		{
			case BUTTON_CANCEL:
				VIEW_SetImage(image_stg1Wght);
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_NEXT:
				VIEW_SetImage(image_calibrate);
				num_lbs = 0;
				num_ounces = 0;

				numSelect = &num_lbs;
				digit = 0;
 				TOUCHSCREEN_ClearState();
				WEIGHT_SetZer0();
				break;
		}
		break;

	case image_calibrate:

		OSMutexFT_use(DRAW_CaliWght);

		WeightKeypad();

		switch(TOUCHSCREEN_IsState())
		{


			case BUTTON_CANCEL:
				VIEW_SetImage(image_stg1Wght);
				TOUCHSCREEN_ClearState();
				break;

			case BUTTON_ENTER:
				VIEW_SetImage(image_stg1Wght);
				if (WEIGHT_ScaleMultiplier(num_lbs*16 + num_ounces) == WEIGHT_SUCCESS)
				{
					VIEW_SetImage(image_EndSuccess);
				}
				else
					VIEW_SetImage(image_EndError);
				TOUCHSCREEN_ClearState();
				break;
		}
		break;

	case image_EndSuccess:
		OSMutexFT_use(DRAW_Success);
		switch(TOUCHSCREEN_IsState())
		{
			case BUTTON_BACK:
				VIEW_SetImage(image_stg1Wght);
				TOUCHSCREEN_ClearState();
				break;
		}
		break;

	case image_EndError:
		OSMutexFT_use(DRAW_Error);
		switch(TOUCHSCREEN_IsState())
		{
			case BUTTON_BACK:
				VIEW_SetImage(image_stg1Wght);
				TOUCHSCREEN_ClearState();
				break;
				}
		break;

	case image_about:
		OSMutexFT_use(DRAW_About);

		switch(TOUCHSCREEN_IsState())
		{
			case BUTTON_BACK:
				VIEW_SetImage(image_stg1Wght);
				TOUCHSCREEN_ClearState();
				break;
		}
		break;



	default:
		view_image = image_init;
		break;

	}
}

void WeightKeypad(void)
{
	switch (TOUCHSCREEN_IsState())
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (digit == 0)
			{
				*numSelect = TOUCHSCREEN_IsState() - 0x30;
			}
			else if (digit == 1)
			{
				*numSelect = 10 * (*numSelect) + (TOUCHSCREEN_IsState() - 0x30);

			}
			else if (digit == 2)
			{
				*numSelect = 10 * (*numSelect) + (TOUCHSCREEN_IsState() - 0x30);
			}

			if (numSelect == &num_lbs)
			{
				if (++digit >= 3)
				{
					digit = 0;
				}
				if (*numSelect > 199)
				{
					num_lbs = 199;
					num_ounces = 15;
				}

			}
			else if (numSelect == &num_ounces)
			{
				if (++digit >= 2)
				{
					digit = 0;
				}
				if (*numSelect > 15)
				{
					*numSelect = 15;
				}
			}
			TOUCHSCREEN_ClearState();
			break;

		case BUTTON_NEXT:
			TOUCHSCREEN_ClearState();
			if (numSelect == &num_lbs)
			{
				numSelect = &num_ounces;
			}
			else if (numSelect == &num_ounces)
			{
				numSelect = &num_lbs;
			}
			digit = 0;

			break;

		case BUTTON_RESET:
			TOUCHSCREEN_ClearState();
			if (numSelect == &num_lbs)
			{
				num_lbs = 0;
			}
			else if (numSelect == &num_ounces)
			{
				num_ounces = 0;
			}

			digit = 0;
			break;
	}
}

//API to move between screen images;
void VIEW_SetImage(ScreenImage image)
{
	view_image = image;
}

uint32_t VIEW_lbs(void)
{
	return(num_lbs);
}

uint32_t VIEW_ounces(void)
{
	return(num_ounces);
}

uint8_t VIEW_ActiveString(void)
{
	if (numSelect == &num_lbs)
	{
		return(1);
	}
	else if (numSelect == &num_ounces)
	{
		return(0);
	}
	else
	{
		return(2);
	}
}

uint32_t VIEW_WeightOz(void)
{
	return(view_weight);
}







