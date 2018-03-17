/*
 * calibrate.h
 *
 *  Created on: Nov 1, 2016
 *      Author: mquaizar
 */

#ifndef CALIBRATE_H_
#define CALIBRATE_H_

extern void CalibrateDisplay(void);
extern void CalibrateEEInfoDelete(void);
extern void CalibrateEEInfoSave(void);
extern void CalibrateDisplayInfo(void);

extern unsigned char Get_CalibrationStatus(void);


#endif /* CALIBRATE_H_ */
