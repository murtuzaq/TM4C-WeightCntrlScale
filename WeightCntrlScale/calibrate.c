/*
 * calibrate.c
 *
 *  Created on: Nov 1, 2016
 *      Author: mquaizar
 */

#include "FT_800.h"
#include "tm4c_eeprom.h"
#include "calibrate.h"

static unsigned char calibrate = 0;


void CalibrateDisplay(void)
{
	CalibrateEEInfoDelete();
	CalibrateDisplayInfo();
}


void CalibrateEEInfoDelete(void)
{
	EEPROM_Write(0, 0);
	EEPROM_Write(0, 1);
	EEPROM_Write(0, 2);
	EEPROM_Write(0, 3);
	EEPROM_Write(0, 4);
	EEPROM_Write(0, 5);
	EEPROM_Write(0, 6);
	EEPROM_Write(0, 7);

	calibrate = 0;
}

void CalibrateEEInfoSave(void)
{
	uint32_t array[8];
	uint32_t i, cksum;

	ReadCalibrationInfo(array);

	//store EEPROM data in array;
	EEPROM_Write(0x1234ABCD, 0); //write in magic key;
	EEPROM_Write(array[0], 1);
	EEPROM_Write(array[1], 2);
	EEPROM_Write(array[2], 3);
	EEPROM_Write(array[3], 4);
	EEPROM_Write(array[4], 5);
	EEPROM_Write(array[5], 6);

	//add checksum value to the end of the array;
	cksum = 0;
	for (i = 0; i < 6; i++)
	{
		cksum += array[i];
	}
	EEPROM_Write(cksum, 7);

	calibrate = 1;

}
void CalibrateDisplayInfo(void) {

	uint32_t i, cksumRd;
	uint32_t arrayEE[8];

	calibrate = 0;

	//check for magic key;
	if (EEPROM_Read(0) == 0x1234ABCD) {
		//Verify checksum;
		for (i = 0; i < 8; i++) {
			arrayEE[i] = EEPROM_Read(i);
		}

		cksumRd = 0;
		for (i = 1; i < 7; i++) {
			cksumRd += arrayEE[i];
		}

		//checksum passed!
		if (EEPROM_Read(7) == cksumRd) {
			LoadCalibrationInfo(&arrayEE[1]);
			calibrate = 1;
		}
	}

	if (calibrate == 0) {
		Calibrate();
	}
}

//API
unsigned char Get_CalibrationStatus(void)
{
	return(calibrate);
}

