/*
 * draw.c
 *
 *  Created on: Apr 10, 2017
 *      Author: mquaizar
 */
#include <stdint.h>
#include "FT_800.h"
#include "weight.h"
#include "sw_utility.h"
#include "draw.h"
#include "about.h"
#include "relay.h"
#include "view.h"
#include "calibrate.h"

const unsigned char* MsgPushbBtnL1 = "PUSH BUTTON";
const unsigned char* MsgPushbBtnL2 = "TO START";
const unsigned char* MsgPushbBtnStp = "PUSH BUTTON TO GO BACK";
const unsigned char* MsgPushbBtnSave = "PUSH BUTTON TO SAVE & GO BACK";
const unsigned char* MsgCalibrationL1 = "PLACE 16 OZ OF WEIGHT HERE FOR";
const unsigned char* MsgCalibrationL2 = "CALIBRATION";
const unsigned char* MsgCalibrationL3 = "PUSH BUTTON TO COMPLETE CALIBRATION";
const unsigned char* MsgSetLimit = "PUSH EACH DIGIT TO SET WEIGHT LIMIT";
const unsigned char* MsgRelayOn = "RELAY ACTIVATED";
const unsigned char* MsgRelayOff = "RELAY DE-ACTIVATED";
const unsigned char* MsgLimitOn = "LIMIT REACHED";
const unsigned char* MsgLimitOff = "LIMIT NOT REACHED";
const unsigned char* strUnitOz = " ounces";
const unsigned char* strUnitLbs = " lbs";
const unsigned char* strPeriod = " . ";



#define COLOR_BLACK			0x00000000
#define COLOR_YELLOW		0x00FFFF00
#define COLOR_GREEN     	0x001BC562
#define COLOR_RED			0x00FF0000
#define COLOR_PEACH			0x00FFAA7F
#define COLOR_REDISH		0x00FF4838
#define COLOR_ORANGE		0x00FF790C
#define COLOR_YELLOWGREEN  	0x0003ff74
#define COLOR_BLUE 			0x000071e2
#define COLOR_WHITE			0x00FFFFFF

#define COLOR_NORMAL		COLOR_BLUE




void DRAW_Init(void)
{
	Initilize_Display();
	TurnOnRotation(0);

	CalibrateDisplayInfo();
	//if calibration status is not save, then save;
	if (!Get_CalibrationStatus())
	{
		CalibrateEEInfoSave();
	}
}

//STAGE 1:
//(init) zero out push button state;
//MSG: PUSH BUTTON TO START
//SW: poll pushbutton;
//SW: poll touch screen;

//STAGE 2:
//MSG: PUSH BUTTON TO STOP
//SW: poll pushbutton;
//SW: Switch Relay ON
//SW: Display & Monitor net weight;
//----Monitor = When Net Weight >= Weight Limit Turn Relay Off;
//----Monitor = When Present Weight < Past weight, Turn Relay Off;
//-------------------------------------------------


void DRAW_Stg1Wght(void)
{
	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);

	FT_DrawText(COLOR_BLACK, 240, 120, 24, (char*)MsgPushbBtnL1);

	FT_DrawText(COLOR_BLACK, 240, 160, 24, (char*)MsgPushbBtnL2);

	FT_DrawButton(COLOR_YELLOWGREEN, 350, 10,  120, 40, 22, "START", BUTTON_START);

	FT_DrawButton(COLOR_NORMAL, 40, 220,  120, 40, 22, "CALIBRATE", BUTTON_CALI);
	FT_DrawButton(COLOR_NORMAL, 180, 220, 120, 40, 22, "SET LIMIT", BUTTON_LIMIT);
	FT_DrawButton(COLOR_NORMAL, 320, 220, 120, 40, 22, "INFO", 	BUTTON_ABOUT);

	FT_CommandDisplay();
}

void DRAW_Stg2Wght(void)
{
	char NumDisplay[80];
	char WghtDisplay[80];

	//busness logic, try to move this out of here;
	//weight = ((float)WEIGHT_Get()) / 10;

	//if (weight >= LIMIT_Get())
	//{
	//	RELAY_Off();
	//}

	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);

	FT_DrawRectangleOutline(COLOR_BLACK, 120, 80, 360, 200);

	FT_DrawButton(COLOR_YELLOWGREEN, 350, 10,  120, 40, 22, "BACK", BUTTON_BACK);

	if (IsRelayActive() == 1)
	{
		FT_DrawText(COLOR_RED, 240, 30, 22, (char*)MsgRelayOn);

		//FT_DrawText(COLOR_RED, 240, 60, 22, (char*)MsgLimitOff);
		WghtDisplay[0] = 0;
		Float2ASCII(WEIGHT_GetLimit()/16, NumDisplay, 0);
		ConcatStr("LIMIT ", NumDisplay, WghtDisplay);
		ConcatStr(WghtDisplay, "lb", WghtDisplay);
		Float2ASCII(WEIGHT_GetLimit()%16, NumDisplay, 0);
		ConcatStr(WghtDisplay,NumDisplay, WghtDisplay);
		ConcatStr(WghtDisplay,"oz ", WghtDisplay);
		ConcatStr(WghtDisplay,"NOT REACHED", WghtDisplay);
		FT_DrawText(COLOR_RED, 240, 60, 22, WghtDisplay);
	}
	else
	{
		FT_DrawText(COLOR_GREEN, 240, 30, 22, (char*)MsgRelayOff);

		//FT_DrawText(COLOR_GREEN, 240, 60, 22, (char*)MsgLimitOn);
		WghtDisplay[0] = 0;
		Float2ASCII(WEIGHT_GetLimit()/16, NumDisplay, 0);
		ConcatStr("LIMIT ", NumDisplay, WghtDisplay);
		ConcatStr(WghtDisplay, "lb", WghtDisplay);
		Float2ASCII(WEIGHT_GetLimit()%16, NumDisplay, 0);
		ConcatStr(WghtDisplay,NumDisplay, WghtDisplay);
		ConcatStr(WghtDisplay,"oz ", WghtDisplay);
		ConcatStr(WghtDisplay,"REACHED", WghtDisplay);
		FT_DrawText(COLOR_GREEN, 240, 60, 22, WghtDisplay);
	}



	//Float2ASCII(WEIGHT_Get(), WghtDisplay, 0);
	//ConcatStr(WghtDisplay, (char*)strUnitOz, WghtDisplay);
	//FT_DrawText(COLOR_BLACK, 240, 140, 24, WghtDisplay);

	WghtDisplay[0] = 0;
	Float2ASCII(VIEW_WeightOz()/16, NumDisplay, 0);
	ConcatStr(NumDisplay, "lb ", WghtDisplay);

	Float2ASCII(VIEW_WeightOz()%16, NumDisplay, 0);

	ConcatStr(WghtDisplay,NumDisplay, WghtDisplay);

	ConcatStr(WghtDisplay,(char*)strUnitOz, WghtDisplay);

	FT_DrawText(COLOR_BLACK, 240, 140, 24, WghtDisplay);


	FT_DrawText(COLOR_BLACK, 240, 230, 22, (char*)MsgPushbBtnStp);

	FT_CommandDisplay();
}


void DRAW_SetLimit(void)
{
	/*
	int8_t strNum2[2];
	int8_t strNum1[2];
	int8_t strNum0[2];

	strNum2[1] = 0;  //null character for string;
	strNum1[1] = 0;
	strNum0[1] = 0;

	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);


	strNum2[0] =  (int8_t)(0x30 + LIMIT_GetHUNDdigit());
	strNum1[0] =  (int8_t)(0x30 + LIMIT_GetTENSdigit());
	strNum0[0] =  (int8_t)(0x30 + LIMIT_GetONESdigit());

	FT_DrawText(COLOR_BLACK, 240, 100, 22, (char*)MsgSetLimit);

	FT_DrawButton(COLOR_PEACH, 85, 145, 45, 60, 31,  (char*)strNum2,  BUTTON_SETBIT2);
	FT_DrawButton(COLOR_PEACH, 135, 145, 45, 60, 31, (char*)strNum1,  BUTTON_SETBIT1);
	FT_DrawButton(COLOR_PEACH, 185, 145, 45, 60, 31, (char*)strNum0,  BUTTON_SETBIT0);

	FT_DrawText(COLOR_BLACK, 280, 175, 24, (char*)strUnitOz);

	FT_DrawButton(COLOR_NORMAL, 335, 155, 100, 40, 22, "RESET", BUTTON_RESET);

	//FT_DrawText(COLOR_BLACK, 240, 230, 22, (char*)MsgPushbBtnSave);
	FT_DrawButton(COLOR_NORMAL, 190, 220, 100, 40, 22, "BACK", BUTTON_BACK);

	FT_CommandDisplay();
	*/

	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);

	Draw_weightkeypad();

	FT_DrawButton(COLOR_NORMAL, 250, 220,  100, 40, 22, "CANCEL", BUTTON_CANCEL);
	FT_DrawButton(COLOR_NORMAL, 360, 220, 100, 40, 22, "ENTER",  BUTTON_ENTER);

	FT_DrawText(COLOR_BLACK, 350, 100, 24, "TYPE IN WEIGHT");
	FT_DrawText(COLOR_BLACK, 350, 160, 22, "PRESS ENTER TO SET LIMIT");


	FT_CommandDisplay();
}

void DRAW_CaliStart(void)
{
	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);

	FT_DrawText(COLOR_BLACK, 240, 100, 24, "START OF CALIBRATION");
	FT_DrawText(COLOR_BLACK, 240, 140, 24, "EMPTY THE WEIGHT SCALE");

	FT_DrawButton(COLOR_NORMAL, 45, 220,  100, 40, 22, "CANCEL", BUTTON_CANCEL);
	FT_DrawButton(COLOR_NORMAL, 335, 220, 100, 40, 22, "NEXT", 	BUTTON_NEXT);

	FT_CommandDisplay();

}
void DRAW_CaliWght(void)
{
	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);

	Draw_weightkeypad();

	FT_DrawButton(COLOR_NORMAL, 250, 220,  100, 40, 22, "CANCEL", BUTTON_CANCEL);
	FT_DrawButton(COLOR_NORMAL, 360, 220, 100, 40, 22, "ENTER",  BUTTON_ENTER);

	FT_DrawText(COLOR_BLACK, 350, 100, 24, "TYPE IN WEIGHT");
	FT_DrawText(COLOR_BLACK, 350, 160, 22, "PUT WEIGHT ON SCALE");
	FT_DrawText(COLOR_BLACK, 350, 200, 22, "PRESS ENTER TO CALIBRATE");


	FT_CommandDisplay();
}

void DRAW_Success(void)
{
	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);


	FT_DrawText(COLOR_BLACK, 240, 100, 31, "SUCCESS");

	FT_DrawButton(COLOR_NORMAL, 190, 220, 100, 40, 22, "EXIT", BUTTON_BACK);

	FT_CommandDisplay();
}

void DRAW_Error(void)
{
	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);


	FT_DrawText(COLOR_BLACK, 240, 100, 31, "ERROR");

	FT_DrawButton(COLOR_NORMAL, 190, 220, 100, 40, 22, "EXIT", BUTTON_BACK);

	FT_CommandDisplay();
}

void DRAW_About(void)
{
	FT_CommandDLStart();

	FT_DrawRectangle(COLOR_WHITE, 0, 0, 480, 272);


	FT_DrawText(COLOR_BLACK, 240, 100, 24, (char*)Legal1);
	FT_DrawText(COLOR_BLACK, 240, 140, 24, (char*)Legal2);
	FT_DrawText(COLOR_BLACK, 240, 180, 24, (char*)Version);


	FT_DrawButton(COLOR_NORMAL, 190, 220, 100, 40, 22, "BACK", BUTTON_BACK);

	FT_CommandDisplay();
}

void Draw_weightkeypad(void)
{
	FT_DrawButton(COLOR_PEACH, 55, 95, 45, 40, 31, "1", '1');
	FT_DrawButton(COLOR_PEACH, 105, 95, 45, 40, 31, "2", '2');
	FT_DrawButton(COLOR_PEACH, 155, 95, 45, 40, 31, "3", '3');

	FT_DrawButton(COLOR_PEACH, 55, 140, 45, 40, 31, "4", '4');
	FT_DrawButton(COLOR_PEACH, 105, 140, 45, 40, 31, "5", '5');
	FT_DrawButton(COLOR_PEACH, 155, 140, 45, 40, 31, "6", '6');

	FT_DrawButton(COLOR_PEACH, 55, 185, 45, 40, 31, "7", '7');
	FT_DrawButton(COLOR_PEACH, 105, 185, 45, 40, 31, "8", '8');
	FT_DrawButton(COLOR_PEACH, 155, 185, 45, 40, 31, "9", '9');

	FT_DrawButton(COLOR_PEACH, 105, 230, 45, 40, 31, "0", '0');

	FT_DrawButton(COLOR_PEACH, 15, 230, 75, 40, 22, "CLEAR", BUTTON_RESET);
	FT_DrawButton(COLOR_PEACH, 160, 230, 75, 40, 22, "SELECT", BUTTON_NEXT);

	if (VIEW_ActiveString())
	{
		FT_DrawPoint(COLOR_BLACK, 80, 20, 24);
	}
	else
	{
		FT_DrawPoint(COLOR_BLACK, 170, 20, 24);
	}

	FT_DrawText(COLOR_BLACK, 80, 40, 24, (char*) strUnitLbs);
	FT_DrawText(COLOR_BLACK, 170, 40, 24, (char*) strUnitOz);

	FT_DrawNumber(COLOR_BLACK, 80, 70, 31, VIEW_lbs());
	FT_DrawNumber(COLOR_BLACK, 170, 70, 31, VIEW_ounces());

}

