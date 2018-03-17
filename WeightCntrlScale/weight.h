/*
 * weight.h
 *
 *  Created on: Apr 5, 2017
 *      Author: mquaizar
 */

#ifndef WEIGHT_H_
#define WEIGHT_H_

#include <stdint.h>

typedef enum {
	WEIGHT_ZERO_ERROR 	= 1,
	WEIGHT_TYPE_ERROR 	= 2,
	WEIGHT_RANGE_ERROR 	= 3,
	WEIGHT_SUCCESS 	 	= 4
}WEIGHT_STATUS;

void WEIGHT_Init(void);
void WEIGHT_Measure(void);
uint32_t WEIGHT_GetValue(void);


WEIGHT_STATUS WEIGHT_ScaleMultiplier(uint32_t weight);
void WEIGHT_SaveScaleMultiplierToEEPROM(void);
void WEIGHT_GetScaleMultiplierFromEEPROM(void);

void WEIGHT_SetZer0(void);
void WEIGHT_SaveScaleZeroToEEPROM(void);
void WEIGHT_GetScaleZeroFromEEPROM(void);



float WEIGHT_IsLimit(void);

WEIGHT_STATUS WEIGHT_SetLimit(uint32_t val);
uint32_t WEIGHT_GetLimit(void);
void WEIGHT_SaveLimitToEEPROM(void);
void WEIGHT_GetLimitFromEEPROM(void);

#endif /* WEIGHT_H_ */
