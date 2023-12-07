/*
 * ecliptar_steering.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef _CONTROL_FPV_H_
#define _CONTROL_FPV_H_

#include <Config/gpio.h>

#include "Control/base.h"

struct FpvProperties
{
  volatile uint16_t	            Pin;
  GPIO_TypeDef*					Port;
  volatile uint32_t               powerLevel;
  volatile float					dutyLevel;
  volatile uint8_t                powerOn :1;
} ;


class FpvController : public ControlHandler
{
public:
	FpvController() {}
	FpvController(GPIO_TypeDef* fpvPort)
	{
		for(int i = 0; i < 3; i++)
			FpvChannel[i]= NULL;
		Initiate(fpvPort);
	}

void					  Initiate(GPIO_TypeDef* fpvPort);

void                      Assign(int controlSetId, uint16_t fpvPin, ControlChannel* fpvChannel);

virtual void              handleControl(uint8_t controlSetId);
virtual void              handleControlStop();

void                      FpvSwitch(FpvProperties* fpv);

ControlChannel*          FpvChannel[3];
FpvProperties            Fpv;

};


#endif /* _CONTROL_FPV_H_ */
