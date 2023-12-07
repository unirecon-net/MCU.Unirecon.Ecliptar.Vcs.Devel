/*
 * ecliptar_drive.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "Drive/base.h"
#include "Drive/ecliptor.h"
#include "Drive/motor.h"
#include "system.h"


class DriveSystem: public AnalogConverter
{
public:
	DriveSystem();

	bool Init(AdcData *hallsensors);
	bool Inited();

	bool Available();

	virtual void ConversionComplete(Ring *stream, u16 size);
	virtual void ConversionHalfComplete(Ring *stream, u16 size);

	void PrintMotionReport();
	void PrintMeasureReport();
	void PrintDynamicsReport();
	void PrintPositionReport();
	void PrintInfoReport();

	DynamicsData 	Dynamics;
	EcliptorDriver LeftEcliptor;
	EcliptorDriver RightEcliptor;
	MotorDriver Motor;
	uint8_t isActive :1;

	uint8_t readyToCompute :1;
	uint8_t newMeasureData :1;

	void ComputativeMeasurements(Ring *buffer, u16 size);

private:

	EcliptorDriver *ecliptors[2];

};

extern DriveSystem *Drive;

#endif /* _DRIVE_H_ */
