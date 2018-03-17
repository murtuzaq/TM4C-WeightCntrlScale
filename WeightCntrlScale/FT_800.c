#include <stdint.h>
#include <string.h>
#include "FT_Platform.h"
#include "FT_800.h"
//#include "../../play_sounds.h"

#define ON          1
#define OFF         0
#define Font        27					// Font Size
#define MAX_LINES   4					// Max Lines allows to Display

#define SPECIAL_FUN     251
#define BACK_SPACE	251				// Back space
#define CAPS_LOCK	252				// Caps Lock
#define NUMBER_LOCK	253				// Number Lock
#define BACK		254				// Exit

#define LINE_STARTPOS	FT_DispWidth/50			// Start of Line
#define LINE_ENDPOS	FT_DispWidth			// max length of the line

void TurnOnRotation(unsigned char Rotate);

/* Global variables for display resolution to support various display panels */
/* Default is WQVGA - 480x272 */
ft_int16_t FT_DispWidth = 480;
ft_int16_t FT_DispHeight = 272;
ft_int16_t FT_DispHCycle =  548;
ft_int16_t FT_DispHOffset = 43;
ft_int16_t FT_DispHSync0 = 0;
ft_int16_t FT_DispHSync1 = 41;
ft_int16_t FT_DispVCycle = 292;
ft_int16_t FT_DispVOffset = 12;
ft_int16_t FT_DispVSync0 = 0;
ft_int16_t FT_DispVSync1 = 10;
ft_uint8_t FT_DispPCLK = 5;
ft_char8_t FT_DispSwizzle = 0;
ft_char8_t FT_DispPCLKPol = 1;

/* Global used for buffer optimization */
Ft_Gpu_Hal_Context_t host,*phost;

/* Boot up for FT800 followed by graphics primitive sample cases */
/* Initial boot up DL - make the back ground green color */
const ft_uint8_t FT_DLCODE_BOOTUP[12] =
{
  255,255,255,2,//GPU instruction CLEAR_COLOR_RGB
  7,0,0,38, //GPU instruction CLEAR
  0,0,0,0,  //GPU instruction DISPLAY
};

// Api to check the touch is detected or not
struct {
  int x;
  int y;
}xyTouch;

uint16_t gtouchState = 0;


ft_uint8_t istouch()
{
  unsigned int xy;
  xy = Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_SCREEN_XY);

  if(!(xy & 0x80008000))
  {
    xyTouch.x = (xy >>16) & 0x0000FFFF;
    xyTouch.y = xy & 0x0000FFFF;

    return(1);
  }
  else
  {
    return(0);
  }
}

unsigned char xyTouch2Loop(void)
{
  unsigned int addr_x;
  unsigned int addr_y;
  unsigned int addr;

  addr_x = (xyTouch.x*16/(30)) >> 4;
  addr_y = (xyTouch.y*16/30) >> 4;

  addr = addr_x + (addr_y - 1)*16;
  return((unsigned char)(addr));
}

unsigned int xyTouch2Pix(void)
{
  unsigned int pix_x;
  unsigned int pix_y;
  unsigned int pix;

  pix_x = xyTouch.x;
  pix_y = xyTouch.y;

  pix =  pix_x + pix_y;
  return(pix);
}

ft_uint32_t Ft_CmdBuffer_Index;


ft_void_t Ft_App_WrCoCmd_Buffer(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t cmd)
{
  Ft_Gpu_Hal_WrCmd32(phost,cmd);

  /* Increment the command index */
  Ft_CmdBuffer_Index += FT_CMD_SIZE;
}

ft_void_t Ft_App_Flush_Co_Buffer(Ft_Gpu_Hal_Context_t *phost)
{
  Ft_CmdBuffer_Index = 0;
}

ft_uint8_t Ft_Gpu_Rom_Font_WH(ft_uint8_t Char,ft_uint8_t font)
{
	ft_uint32_t ptr,Wptr;
	ft_uint8_t Width=0;
	ptr = Ft_Gpu_Hal_Rd32(phost,0xffffc);
// Read Width of the character
	Wptr = (ptr + (148 * (font- 16)))+Char;	// (table starts at font 16)
	Width = Ft_Gpu_Hal_Rd8(phost,Wptr);
	return Width;
}

// Api to play Key sound
ft_void_t FT_Play_Sound(ft_uint16_t sound,ft_uint8_t volume)
{
  Ft_Gpu_Hal_Wr8(phost,REG_VOL_SOUND,volume);
  Ft_Gpu_Hal_Wr16(phost,REG_SOUND,sound);
  Ft_Gpu_Hal_Wr8(phost,REG_PLAY,1);
}


void FT_StopSound(void)
{
	  Ft_Gpu_Hal_Wr16(phost,REG_SOUND,0);
	  Ft_Gpu_Hal_Wr8(phost,REG_PLAY,1);
}

unsigned char FT_IsSound(void)
{
	return(Ft_Gpu_Hal_Rd8(phost,REG_PLAY));
}

void Touch_Mode_On()
{
    Ft_Gpu_Hal_Wr8(phost,REG_TOUCH_MODE, 0x3);
}

void Touch_Mode_Off()
{
    Ft_Gpu_Hal_Wr8(phost,REG_TOUCH_MODE, 0x0);
}

void TurnOnRotation(unsigned char Rotate)
{
  if (Rotate)
  {
    Ft_Gpu_Hal_Wr8(phost,REG_ROTATE, 0x01);
  }
}

ft_uint8_t Read_Tag()
{
  return(Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG));
}

ft_uint32_t Read_TouchPresure(void)
{
  static unsigned int Read_touch;
  Read_touch = Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_RZ);

  return Read_touch;
}
ft_uint32_t Read_TouchScreen(void)
{
  static unsigned int Read_touch;
  Read_touch = Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_TAG_XY);

  return Read_touch;
}

ft_uint32_t Read_Touch_Direct(void)
{
  static unsigned int Read_touch;
  Read_touch = Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_DIRECT_XY);

  return Read_touch;
}

ft_uint32_t Read_Touch_Direct_Z1Z2(void)
{
  static unsigned int Read_touch;
  Read_touch = Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_DIRECT_Z1Z2);

  return Read_touch;
}


void Clear_Touch_Tag_X_Y(void)
{
  Ft_Gpu_Hal_Wr16(phost,REG_TAG_X, 140);
  Ft_Gpu_Hal_Wr16(phost,REG_TAG_Y, 420);
}

uint8_t ReadPushButton(void)
{

	static KEY_STATE KeyPressState = NO_PRESS_DETECT;
	static uint8_t keyValue = 0;
	static uint8_t holdcount = 0;
	uint8_t keypress = 0;

	keypress = Read_Tag();

	//PlaySound_ButtonStatus(keypress);

	switch (KeyPressState)
	{
	case NO_PRESS_DETECT:
		if (keypress > 0 && keypress < 0xFF)
		{
			holdcount = 0;
			keyValue = keypress;
			KeyPressState = HOLD_KEY_DETECT;
		}
		else
		{
			gtouchState = 0;
		}
		break;

	case HOLD_KEY_DETECT:
		//hold for atleast three cycle;
		if (holdcount < 1)
		{
			if (keypress == keyValue)
			{
				holdcount++;
			}
			else
			{
				KeyPressState = NO_PRESS_DETECT;
			}

		}
		else
		{
			if (keypress == 0)
			{
				holdcount = 0;
				KeyPressState = RELEASE_KEY_DETECT;
			}
			else if (keypress != keyValue)
			{
				holdcount = 0;
				KeyPressState = NO_PRESS_DETECT;
			}
			else if (keypress == keyValue)
			{
				gtouchState = keyValue + 0x0100;
				//do nothing;
			}

		}
		break;

	case RELEASE_KEY_DETECT:
		if (holdcount < 1) {
			if (keypress != 0) {
				KeyPressState = NO_PRESS_DETECT;
			}
			holdcount++;
		} else {
			//detection found, start over;
			KeyPressState = NO_PRESS_DETECT;
			gtouchState = keyValue;
			return (keyValue);
		}
	}
	return (0);

}


void Calibrate(void)
{
  Ft_Gpu_CoCmd_Dlstart(phost);
  Ft_App_WrCoCmd_Buffer(phost,CLEAR(1,1,1));
  Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(255,255,255));
  Ft_Gpu_CoCmd_Text(phost,FT_DispWidth/2,FT_DispHeight/2,26,OPT_CENTERX|OPT_CENTERY,"Please tap on a dot");
  Ft_Gpu_CoCmd_Calibrate(phost,0);
}


void Initilize_Display()
{
  Ft_Gpu_Hal_Sleep(100);

  ft_uint8_t chipid;

  Ft_Gpu_HalInit_t halinit;

  halinit.TotalChannelNum = 1;
  Ft_Gpu_Hal_Init(&halinit);
  host.hal_config.channel_no = 0;
  host.hal_config.spi_clockrate_khz = 4000; //in KHz

  Ft_Gpu_Hal_Open(&host);

  phost = &host;

  /* Do a power cycle for safer side */
  Ft_Gpu_Hal_Powercycle(phost,FT_TRUE);
  Ft_Gpu_Hal_Rd16(phost,RAM_G);
  /* Set the clk to external clock */
  Ft_Gpu_HostCommand(phost,FT_GPU_EXTERNAL_OSC);
  Ft_Gpu_Hal_Sleep(10);


  /* Switch PLL output to 48MHz */
  Ft_Gpu_HostCommand(phost,FT_GPU_PLL_48M);
  Ft_Gpu_Hal_Sleep(10);

  /* Do a core reset for safer side */
  Ft_Gpu_HostCommand(phost,FT_GPU_CORE_RESET);
  //Ft_Gpu_CoreReset(phost);

  /* Access address 0 to wake up the FT800 */
  Ft_Gpu_HostCommand(phost,FT_GPU_ACTIVE_M);

  //Read Register ID to check if FT800 is ready.
  chipid = Ft_Gpu_Hal_Rd8(phost, REG_ID);
  while(chipid != 0x7C)
  {
    chipid = Ft_Gpu_Hal_Rd8(phost, REG_ID);
  }

  /* Configuration of LCD display */
  Ft_Gpu_Hal_Wr16(phost, REG_HCYCLE, FT_DispHCycle);
  Ft_Gpu_Hal_Wr16(phost, REG_HOFFSET, FT_DispHOffset);
  Ft_Gpu_Hal_Wr16(phost, REG_HSYNC0, FT_DispHSync0);
  Ft_Gpu_Hal_Wr16(phost, REG_HSYNC1, FT_DispHSync1);
  Ft_Gpu_Hal_Wr16(phost, REG_VCYCLE, FT_DispVCycle);
  Ft_Gpu_Hal_Wr16(phost, REG_VOFFSET, FT_DispVOffset);
  Ft_Gpu_Hal_Wr16(phost, REG_VSYNC0, FT_DispVSync0);
  Ft_Gpu_Hal_Wr16(phost, REG_VSYNC1, FT_DispVSync1);
  Ft_Gpu_Hal_Wr8(phost, REG_SWIZZLE, FT_DispSwizzle);
  Ft_Gpu_Hal_Wr8(phost, REG_PCLK_POL, FT_DispPCLKPol);
  Ft_Gpu_Hal_Wr8(phost, REG_PCLK,FT_DispPCLK);//after this display is visible on the LCD
  Ft_Gpu_Hal_Wr16(phost, REG_HSIZE, FT_DispWidth);
  Ft_Gpu_Hal_Wr16(phost, REG_VSIZE, FT_DispHeight);

  /* Initially fill both ping and pong buffer */
  Ft_Gpu_Hal_Wr8(phost, REG_GPIO_DIR,0xff);
  Ft_Gpu_Hal_Wr8(phost, REG_GPIO,0x0ff);
  /* Touch configuration - configure the resistance value to 1200 - this value is specific to customer requirement and derived by experiment */
  Ft_Gpu_Hal_Wr16(phost, REG_TOUCH_RZTHRESH,1200);

  /*clear the screen here*/
  Ft_Gpu_Hal_WrMem(phost, RAM_DL,(ft_uint8_t *)FT_DLCODE_BOOTUP,sizeof(FT_DLCODE_BOOTUP));
  Ft_Gpu_Hal_Wr8(phost, REG_DLSWAP,DLSWAP_FRAME);

  Ft_Gpu_Hal_Sleep(1000);//wait 1 second before proceeding on.

}





void LoadCalibrationInfo(uint32_t *CalArray)
{
  Ft_Gpu_Hal_Wr32(phost,REG_TOUCH_TRANSFORM_A, CalArray[0]);
  Ft_Gpu_Hal_Wr32(phost,REG_TOUCH_TRANSFORM_B, CalArray[1]);
  Ft_Gpu_Hal_Wr32(phost,REG_TOUCH_TRANSFORM_C, CalArray[2]);
  Ft_Gpu_Hal_Wr32(phost,REG_TOUCH_TRANSFORM_D, CalArray[3]);
  Ft_Gpu_Hal_Wr32(phost,REG_TOUCH_TRANSFORM_E, CalArray[4]);
  Ft_Gpu_Hal_Wr32(phost,REG_TOUCH_TRANSFORM_F, CalArray[5]);

}

void ReadCalibrationInfo(uint32_t *CalArray)
{
  CalArray[0] =  Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_TRANSFORM_A);
  CalArray[1] =  Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_TRANSFORM_B);
  CalArray[2] =  Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_TRANSFORM_C);
  CalArray[3] =  Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_TRANSFORM_D);
  CalArray[4] =  Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_TRANSFORM_E);
  CalArray[5] =  Ft_Gpu_Hal_Rd32(phost,REG_TOUCH_TRANSFORM_F);

}

void FT_CommandDLStart(void)
{
    Ft_Gpu_CoCmd_Dlstart(phost);
    Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(0,0,0));
    Ft_App_WrCoCmd_Buffer(phost,CLEAR(1,1,1));
}

void FT_CommandDisplay(void)
{
  Ft_App_WrCoCmd_Buffer(phost,DISPLAY());
  Ft_Gpu_CoCmd_Swap(phost);
}

void FT_DrawKeyPad(uint32_t color, uint16_t x,uint16_t y, uint16_t width, uint16_t height, uint16_t font, char *keys)
{
  Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
  Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0,0,0));
  Ft_Gpu_CoCmd_FgColor(phost,color);
  Ft_Gpu_CoCmd_GradColor(phost,0xffffff);
  Ft_Gpu_CoCmd_Keys(phost, x,y,width,height, font,0, (char const*)keys);
  Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());
}

void FT_DrawButton(uint32_t color, uint16_t x,uint16_t y, uint16_t width, uint16_t height, uint16_t font, char *txt, uint8_t tag)
{
  uint8_t btnPress = 0;
  uint8_t btnActive = 0;
  Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
  Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0,0,0));

   btnPress =((uint8_t)gtouchState & 0x00FF);
   btnActive = ((gtouchState & 0x0100) == 0x0100);

  if ((btnPress == tag) && (btnActive == 1))
  {
    color = 0x003870;
  }
  Ft_Gpu_CoCmd_FgColor(phost,color);
  Ft_Gpu_CoCmd_GradColor(phost,0xffffff);
  Ft_App_WrCoCmd_Buffer(phost,TAG(tag));





  Ft_Gpu_CoCmd_Button(phost, x,y, width,height, font, 0,(char const*)txt);
  Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());
}

void FT_DrawPoint(uint32_t color, uint16_t x1, uint16_t y1, uint16_t size)
{

	uint8_t red;
	uint8_t green;
	uint8_t blue;

	ColorToRGB(color, &red, &green, &blue);

	Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(red,green,blue));
	Ft_App_WrCoCmd_Buffer(phost,POINT_SIZE(size));
	Ft_App_WrCoCmd_Buffer(phost,BEGIN(POINTS));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(x1*16,y1*16));
  	Ft_App_WrCoCmd_Buffer(phost,END());
	Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());

}


void FT_DrawRectangle(uint32_t color, uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2) {

	uint8_t red;
	uint8_t green;
	uint8_t blue;

	ColorToRGB(color, &red, &green, &blue);

	Ft_App_WrCoCmd_Buffer(phost, SAVE_CONTEXT());
	Ft_App_WrCoCmd_Buffer(phost, COLOR_RGB(red, green, blue));
	Ft_App_WrCoCmd_Buffer(phost, LINE_WIDTH(16));
	Ft_App_WrCoCmd_Buffer(phost, BEGIN(RECTS));
	Ft_App_WrCoCmd_Buffer(phost, VERTEX2F(x1 * 16, y1 * 16));
	Ft_App_WrCoCmd_Buffer(phost, VERTEX2F(x2 * 16, y2 * 16));
	Ft_App_WrCoCmd_Buffer(phost, END());
	Ft_App_WrCoCmd_Buffer(phost, RESTORE_CONTEXT());

}
void FT_DrawText(uint32_t color,uint16_t x, uint16_t y, uint8_t font, char* txt)
{

	uint8_t red;
	uint8_t green;
	uint8_t blue;

	ColorToRGB(color, &red, &green, &blue);

	Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(red,green,blue));
	Ft_Gpu_CoCmd_Text(phost,x,y,font, 1536, (char const*)txt);
	Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());
}

void FT_DrawNumber(uint32_t color, uint16_t x, uint16_t y, uint8_t font, uint32_t number)
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	ColorToRGB(color, &red, &green, &blue);

	Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(red,green,blue));
	Ft_Gpu_CoCmd_Number(phost, x,y,font, 1536, number);
	Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());
}

void FT_DrawLine(uint32_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	ColorToRGB(color, &red, &green, &blue);

	Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
	Ft_App_WrCoCmd_Buffer(phost,LINE_WIDTH(16));
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(red,green,blue));
	Ft_App_WrCoCmd_Buffer(phost,BEGIN(LINES));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(x1*16,y1*16));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(x2*16,y2*16));
	Ft_App_WrCoCmd_Buffer(phost,END());
	Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());
}

void FT_DrawThinLine(uint32_t color,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	ColorToRGB(color, &red, &green, &blue);

	Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
	Ft_App_WrCoCmd_Buffer(phost,LINE_WIDTH(8));
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(red,green,blue));
	Ft_App_WrCoCmd_Buffer(phost,BEGIN(LINES));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(x1*16,y1*16));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(x2*16,y2*16));
	Ft_App_WrCoCmd_Buffer(phost,END());
	Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());
}

void FT_DrawConnect(uint32_t color,uint16_t x, uint16_t ndata, uint8_t deltaX, uint8_t* pdata)
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint16_t i, xp;

	ColorToRGB(color, &red, &green, &blue);

	Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(red,green,blue));




	Ft_App_WrCoCmd_Buffer(phost,LINE_WIDTH(4));
	Ft_App_WrCoCmd_Buffer(phost,BEGIN(LINES));

	//Ft_App_WrCoCmd_Buffer(phost,POINT_SIZE(50));

	xp = x;
	for(i = 0; i < (ndata -1); i++)
	{
		Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(xp*16,(260 - pdata[i])*16));

		xp = xp + deltaX;

		Ft_App_WrCoCmd_Buffer(phost,VERTEX2F((xp)*16,(260 - pdata[i + 1])*16));

		//Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(xp*16,(270 - pdata[i])*16));
		//xp++;

	}

	Ft_App_WrCoCmd_Buffer(phost,END());
	Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());





}
void FT_DrawRectangleOutline(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{

	FT_DrawLine(color, x1, y1, x2, y1);
	FT_DrawLine(color, x2, y1, x2, y2);
	FT_DrawLine(color, x2, y2, x1, y2);
	FT_DrawLine(color, x1, y2, x1, y1);

}


void ColorToRGB(uint32_t color, uint8_t* r, uint8_t* g, uint8_t* b)
{
	uint32_t r32, g32, b32;

	r32 = (color & (0xFF << 16)) >> 16;
	g32 = (color & (0xFF << 8)) >> 8;
	b32 = (color & (0xFF << 0)) >> 0;

	*r = (uint8_t)r32;
	*g = (uint8_t)g32;
	*b = (uint8_t)b32;
}









