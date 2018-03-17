/*
 * draw.h
 *
 *  Created on: Apr 10, 2017
 *      Author: mquaizar
 */

#ifndef DRAW_H_
#define DRAW_H_


#define BUTTON_CALI			0x10
#define BUTTON_LIMIT		0x1A
#define BUTTON_ABOUT		0x1B
#define BUTTON_SETBIT2		0x1C
#define BUTTON_SETBIT1		0x1D
#define BUTTON_SETBIT0		0x1E
#define BUTTON_RESET		0x1F
#define BUTTON_BACK			0x20
#define BUTTON_NEXT   		0x21
#define BUTTON_ENTER        0x22
#define BUTTON_CANCEL       0x23
#define BUTTON_START		0x24



void DRAW_Init(void);
void DRAW_Stg1Wght(void);
void DRAW_Stg2Wght(void);
void DRAW_About(void);
void DRAW_CaliStart(void);
void DRAW_CaliWght(void);
void DRAW_SetLimit(void);
void DRAW_Success(void);
void DRAW_Error(void);

//helper function;
void Draw_weightkeypad(void);




#endif /* DRAW_H_ */
