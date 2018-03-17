/*
 * button.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Murtuza
 */

#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_



uint8_t TOUCHSCREEN_Read(void);
void TOUCHSCREEN_statemachine(void);

void TOUCHSCREEN_ClearState(void);
uint8_t TOUCHSCREEN_IsState(void);


#endif /* TOUCHSCREEN_H_ */
