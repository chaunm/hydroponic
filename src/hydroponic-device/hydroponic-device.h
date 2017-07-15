/*
 * hydroponic-device.h
 *
 *  Created on: Jul 15, 2017
 *      Author: chaunm
 */

#ifndef _HYDROPONIC_DEVICE_H_
#define _HYDROPONIC_DEVICE_H_
#include "serialcommunication.h"
#include "actor.h"

#pragma pack(1)
typedef struct stHydroponicDevice {
	PSERIAL serialPort;
	PACTOR actor;
}HYDROPONIC_DEVICE, *PHYDROPONIC_DEVICE;

BYTE DeviceStart(PACTOROPTION actorOption, char* SerialPort);

#endif /* _HYDROPONIC_DEVICE_H_ */
