
#ifndef _FT_PLATFORM_H_
#define _FT_PLATFORM_H_

#include <stdint.h>
#include "FT_DataTypes.h"
#include "FT_Gpu_Hal.h"
#include "FT_Gpu.h"
#include "FT_CoPro_Cmds.h"

//#define SAMAPP_DISPLAY_QVGA
#define SAMAPP_DISPLAY_WQVGA

#define ORIENTATION_PORTRAIT
//#define ORIENTATION_LANDSCAPE

void FT_Init(void);
void FT800_CS_SET(void);
void FT800_CS_CLEAR(void);
void FT800_PD_N_SET(void);
void FT800_PD_N_CLEAR(void);
uint8_t FT_spiRdWt(uint8_t data);
void FT_Delay(uint16_t ms);

//#include "FT_DataTypes.h"
//#include "FT_Gpu_Hal.h"
//#include "FT_Gpu.h"
//#include "FT_CoPro_Cmds.h"



#endif /*_FT_PLATFORM_H_*/
/* Nothing beyond this*/




