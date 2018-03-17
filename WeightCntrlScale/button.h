/*
 * button.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Murtuza
 */

#ifndef BUTTON_H_
#define BUTTON_H_


typedef enum {
	BUTTON_ACTIVE =  0,
	BUTTON_RELEASE = 1
} BUTTON_STATE;

void BUTTON_Init(void);
BUTTON_STATE BUTTON_Read(void);
void BUTTON_statemachine(void);

void BUTTON_ClearState(void);
BUTTON_STATE BUTTON_IsState(void);


#endif /* BUTTON_H_ */
