/*
 * config_tm4c.c
 *
 *  Created on: Feb 19, 2017
 *      Author: Murtuza
 */

#include <stdint.h>
#include "tm4c_eeprom.h"
#include "relay.h"


void CONFIG_Init(void)
{
	RELAY_Init();
	EEPROM_Init();

}

