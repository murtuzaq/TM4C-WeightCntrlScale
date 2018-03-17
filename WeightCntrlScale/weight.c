/*
 * weight.c
 *
 *  Created on: Apr 5, 2017
 *      Author: mquaizar
 */

#include "tm4c_adc1.h"
#include "tm4c_port.h"
#include "hx711.h"
#include "tm4c_eeprom.h"
#include "weight.h"

#define EEPROM_LIMIT_OFFSET	10
#define EEPROM_SCALE_MULTIPLIER		11
#define EEPROM_ZERO_LIMIT			12


uint32_t weight_limit = 0;
uint32_t weight_value = 0;
uint32_t weight_base = 0;
float weight_scalemultipler = 0;


void WEIGHT_Init(void)
{
	ADC1_Init();
	HX711_Init();
}


 void WEIGHT_Measure(void)
{

	 // uint32_t new_value = 0;

	 weight_value = HX711_Run();
/*
	 new_value = HX711_Run();

    if ((new_value > (weight_value + 8))
    		|| (new_value < (weight_value - 8)))
    {
    	weight_value = new_value;
    }
    */

}

 //take the return value / 10 to get 1/10 of precision in ounces
  uint32_t WEIGHT_GetValue(void)
{
	 uint32_t value;

	if (weight_value > weight_base)
	{
		//value = weight_value - weight_base_adc;
		//value = value *16;
		//value = value *10; //precision by 1 decimal place;
		//value = value/((weight_set16oz - weight_base_adc));
		value = weight_value  - weight_base;
		value = (uint32_t)((float)(value * weight_scalemultipler + 0.5));

	}
	else
	{
		weight_base = weight_value;
		value = 0;
	}
	//return(value);
	return(value);
	//return(weight_value);
}

void WEIGHT_SaveScaleMultiplierToEEPROM(void)
{
	uint32_t val;

	val = (uint32_t)(10000000*weight_scalemultipler);
	EEPROM_Write(val, EEPROM_SCALE_MULTIPLIER);
}

void WEIGHT_GetScaleMultiplierFromEEPROM(void)
{
	uint32_t val = 0;
  	val = EEPROM_Read(EEPROM_SCALE_MULTIPLIER);

  	weight_scalemultipler = (float)val/10000000;
}


WEIGHT_STATUS WEIGHT_ScaleMultiplier(uint32_t weight)
{
	if (weight_value > weight_base)
	{
		if (weight > 0)
		{
			weight_scalemultipler = (float)weight/((float)weight_value - (float)weight_base);
		}
		else
		{
			return(WEIGHT_TYPE_ERROR);
		}
	}
	else
	{
		return(WEIGHT_ZERO_ERROR);
	}


	WEIGHT_SaveScaleZeroToEEPROM();
	WEIGHT_SaveScaleMultiplierToEEPROM();

	return(WEIGHT_SUCCESS);
}

void WEIGHT_SetZer0(void)
{
	weight_base = weight_value;
}

void WEIGHT_SaveScaleZeroToEEPROM(void)
{
	EEPROM_Write(weight_base, EEPROM_ZERO_LIMIT);
}

void WEIGHT_GetScaleZeroFromEEPROM(void)
{
	uint32_t val = 0;
  	val = EEPROM_Read(EEPROM_ZERO_LIMIT);

  	weight_base = val;
}

WEIGHT_STATUS WEIGHT_SetLimit(uint32_t val)
{
	if(val > 0 && val < 3200)
	{
		weight_limit = val;
		WEIGHT_SaveLimitToEEPROM();
		return(WEIGHT_SUCCESS);
	}
	else
	{
		return(WEIGHT_RANGE_ERROR);
	}
}

uint32_t WEIGHT_GetLimit(void)
{
	return(weight_limit);
}

void WEIGHT_SaveLimitToEEPROM(void)
{
	EEPROM_Write((uint32_t)(weight_limit), EEPROM_LIMIT_OFFSET);
}

void WEIGHT_GetLimitFromEEPROM(void)
{
	uint32_t val = 0;
	val = EEPROM_Read(EEPROM_LIMIT_OFFSET);

	//set weight limit from eeprom
	weight_limit = val;

}


