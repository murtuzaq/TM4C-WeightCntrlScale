/*
 * view.h
 *
 *  Created on: Apr 4, 2017
 *      Author: mquaizar
 */

#ifndef VIEW_H_
#define VIEW_H_

typedef enum {
	image_init,
	image_stg1Wght, //stage 1 Weight;
	image_stg2Wght, //stage 2 Weight;
	image_menu,
	image_setlimit,
	image_calibrStart,
	image_calibrate,
	image_EndSuccess,
	image_EndError,
	image_about
}ScreenImage;

void VIEW_ImageState(void);
void VIEW_SetImage(ScreenImage image);

uint32_t VIEW_lbs(void);
uint32_t VIEW_ounces(void);
uint8_t VIEW_ActiveString(void);

uint32_t VIEW_WeightOz(void);

uint8_t VIEW_LimitTENSdigit(void);
uint8_t VIEW_LimitONESdigit(void);
uint8_t VIEW_LimitTENTHdigit(void);

void WeightKeypad(void);

#endif /* VIEW_H_ */
