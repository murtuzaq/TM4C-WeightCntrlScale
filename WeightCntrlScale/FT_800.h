#ifndef __FT843_H__
#define __FT843_H__

#include <stdint.h>
#include "FT_Platform.h"


typedef enum {
	NO_PRESS_DETECT,
	KEY_PRESS_DETECT,
	HOLD_KEY_DETECT,
	RELEASE_KEY_DETECT,
}KEY_STATE;

ft_void_t Ft_App_Flush_Co_Buffer(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_App_WrCoCmd_Buffer(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t cmd);
ft_void_t FT_Play_Sound(ft_uint16_t sound,ft_uint8_t volume);

void FT_StopSound(void);
unsigned char FT_IsSound(void);

ft_uint8_t Ft_Gpu_Rom_Font_WH(ft_uint8_t Char,ft_uint8_t font);
ft_uint8_t istouch(void);


void Initilize_Display();
void Calibrate(void);
void DrawHomeMenu(void);
void DrawLoopScan(unsigned char update_screen, unsigned char scan_addr, unsigned char select_addr);
void InitDeviceInBlock(void);
void LoadCalibrationInfo(uint32_t *CalArray);
void ReadCalibrationInfo(uint32_t *CalArray);
ft_uint8_t Read_Tag();
ft_uint8_t Read_Keys();
ft_uint32_t Read_TouchScreen(void);
ft_uint32_t Read_TouchPresure(void);
void InitBoxLocation(void);
void Touch_Mode_On(void);
void Touch_Mode_Off(void);
void Clear_Touch_Tag_X_Y(void);
void TurnOnRotation(unsigned char Rotate);
ft_uint32_t Read_Touch_Direct(void);
ft_uint32_t Read_Touch_Direct_Z1Z2(void);

uint8_t ReadPushButton(void);
void DrawButton(void);
void DrawMoreDeviceMatrix(void);
unsigned int xyTouch2Pix(void);

void FT_CommandDLStart(void);
void FT_CommandDisplay(void);

void ColorToRGB(uint32_t color, uint8_t* r, uint8_t* g, uint8_t* b);
void FT_DrawKeyPad(uint32_t color, uint16_t x,uint16_t y, uint16_t width, uint16_t height, uint16_t font, char *keys);
void FT_DrawButton(uint32_t color, uint16_t x,uint16_t y, uint16_t width, uint16_t height, uint16_t font, char *txt, uint8_t tag);
//void FT_DrawRectangle(uint8_t red, uint8_t green, uint8_t blue, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void FT_DrawRectangle(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
//void FT_DrawLine(uint8_t red, uint8_t green, uint8_t blue,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void FT_DrawLine(uint32_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void FT_DrawThinLine(uint32_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
//void FT_DrawText(uint8_t red, uint8_t green, uint8_t blue,uint16_t x, uint16_t y, uint8_t font, uint8_t* txt);
void FT_DrawText(uint32_t color,uint16_t x, uint16_t y, uint8_t font, char* txt);
//void FT_DrawNumber(uint8_t red, uint8_t green, uint8_t blue,uint16_t x, uint16_t y, uint8_t font, uint32_t number);
void FT_DrawNumber(uint32_t color,uint16_t x, uint16_t y, uint8_t font, uint32_t number);
//void FT_DrawRectangleOutline(uint8_t red, uint8_t green, uint8_t blue, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void FT_DrawRectangleOutline(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void FT_DrawConnect(uint32_t color,uint16_t x, uint16_t ndata, uint8_t deltaX, uint8_t* pdata);

void FT_DrawPoint(uint32_t color, uint16_t x1, uint16_t y1, uint16_t size);
#endif
