/*
 * osMutex_FT.h
 *
 *  Created on: Apr 10, 2017
 *      Author: mquaizar
 */

#ifndef OSMUTEX_FT_H_
#define OSMUTEX_FT_H_

void OSMutexFT_Init(void);
void OSMutexFT_use(void (*ftfunc)(void));
uint8_t OSMutexFT_u8use(uint8_t (*u8ftfunc)(void));


#endif /* OSMUTEX_FT_H_ */
