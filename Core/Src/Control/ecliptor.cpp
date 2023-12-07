/*
 * ecliptar_steering.c
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#include "Control/ecliptor.h"

EcliptorController::EcliptorController(EcliptorDriver* ecliptor, int orderId)
{
    Driver = ecliptor;
    EccentratorControl = &ecliptor->Eccentrator;
    RotatorControl = &ecliptor->Rotator;
    rotatorMixDirection = left;
    eccntrMixDirection = left;
    attachController(orderId);

}

void        EcliptorController::assign(int controlSetId, int eccntrChannelNo, int eccntrMixChannelNo, MixDirection eccntrMix, int rotatorChannelNo, int rotatorMixChannelNo, MixDirection rotatorMix)
{
    assign(controlSetId, RC->Channels[eccntrChannelNo - 1], RC->Channels[eccntrMixChannelNo - 1], eccntrMix,
        RC->Channels[rotatorChannelNo - 1], RC->Channels[rotatorMixChannelNo - 1], rotatorMix);
}
void        EcliptorController::assign(int controlSetId, ControlChannel* eccntrChannel, ControlChannel* eccntrMixChannel, MixDirection eccntrMix, ControlChannel* rotatorChannel, ControlChannel* rotatorMixChannel, MixDirection rotatorMix)
{
    for(int i = 0; i < 3; i++)
    {
	    	EccntrChannel[i] = NULL;
	    	EccntrMixChannel[i] = NULL;

	    	RotatorChannel[i] = NULL;
	    	RotatorMixChannel[i] = NULL;
	}

	EccntrChannel[controlSetId] = eccntrChannel;
    EccntrMixChannel[controlSetId] = eccntrMixChannel;
    eccntrMixDirection = eccntrMix;

    RotatorChannel[controlSetId] = rotatorChannel;
    RotatorMixChannel[controlSetId] = rotatorMixChannel;
    rotatorMixDirection = rotatorMix;
}

void        EcliptorController::handleControl(uint8_t controlSetId)
{
    handleEccntrControl(controlSetId);

    handleRotatorControl(controlSetId);

    //Driver->computeMotion();
}
void        EcliptorController::handleControlStop()
{
    Driver->PowerDuty(0);
    Driver->RotatePercents(0);
}

void        EcliptorController::handleEccntrControl(uint8_t controlSetId)
{
	 uint8_t dataChanged = 0;
	 ControlChannel* channel = EccntrChannel[controlSetId];

	 if (channel != NULL)
	 {
	     dataChanged += channel->available;
	     float percents = channel->controlValue;
	     ControlChannel* mixchannel = EccntrMixChannel[controlSetId];
	     if (mixchannel != NULL)
	     {
	         dataChanged += mixchannel->available;
	         if (dataChanged > 0)
	         {
	             percents = constrain(percents -
	            		 (mixchannel->controlValue * eccntrMixDirection),
						 	 0.00F, 100.00F);
	         }
	     }
	     if (dataChanged > 0)
	         Driver->PowerDuty(percents);
	 }
}

void        EcliptorController::handleRotatorControl(uint8_t controlSetId)
{
	  uint8_t dataChanged = 0;
	  ControlChannel* channel = RotatorChannel[controlSetId];
	  if (channel != NULL)
	  {
	      dataChanged += channel->available;
	      float percents = channel->controlValue;
	      ControlChannel* mixchannel = RotatorMixChannel[controlSetId];
	      if (mixchannel != NULL)
	      {
	          dataChanged += mixchannel->available;
	          if (dataChanged > 0)
	          {
	              percents = constrain(percents -
	            		  (mixchannel->controlValue * rotatorMixDirection),
						  	  -100.00F, 100.00F);
	          }
	      }
	      if (dataChanged > 0)
	          Driver->RotatePercents(percents);
	  }
}



