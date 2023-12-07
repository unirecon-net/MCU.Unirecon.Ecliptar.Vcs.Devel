/*
 * ecliptar_steering.c
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#include "Control/fpv.h"

void FpvController::Initiate(GPIO_TypeDef* fpvPort)
{
    attachController(5);
    Fpv.Port = fpvPort;
}

void        FpvController::Assign(int controlSetId, uint16_t fpvPin, ControlChannel* fpvChannel)
{
	for(int i = 0; i < 3; i++)
	{
		FpvChannel[i] = NULL;
	}
    Fpv.Pin = fpvPin;
    FpvChannel[controlSetId] = fpvChannel;
}

void        FpvController::handleControl(uint8_t controlSetId)
{
    ControlChannel* fpv = FpvChannel[controlSetId];
    if (fpv != NULL)
    {
        if (fpv->available)
        {
            int state = fpv->controlValue;
            if ((state < 10 && Fpv.powerOn) ||
                (state > 10 && !Fpv.powerOn))
                FpvSwitch(&Fpv);
        }
    }
}
void        FpvController::handleControlStop()
{
    if (Fpv.powerOn)
        FpvSwitch(&Fpv);
}

void        FpvController::FpvSwitch(FpvProperties* fpv)
{
    if (fpv->powerOn)
    {
    	fpv->powerOn = false;
        HAL_GPIO_WritePin(fpv->Port, fpv->Pin, LOW);
    }
    else
    {
    	fpv->powerOn = true;
        HAL_GPIO_WritePin(fpv->Port, fpv->Pin, HIGH);
    }
}




