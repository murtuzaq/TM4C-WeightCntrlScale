/*
 * relay.c
 *
 *  Created on: Apr 5, 2017
 *      Author: mquaizar
 */

#include "tm4c_port.h"

static uint8_t relaystate;

void RELAY_Init(void)
{
	PORT_PE3out_Init();
	relaystate  = 0;
}


void RELAY_On(void)
{
	PORT_PE3out_Set();
	relaystate = 1;
}

void RELAY_Off(void)
{
	PORT_PE3out_Clear();
	relaystate  = 0;
}

uint8_t IsRelayActive(void)
{
	return(relaystate);
}
