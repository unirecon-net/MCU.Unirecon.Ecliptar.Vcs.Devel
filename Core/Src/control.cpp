/*
 * ecliptar_steering.c
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */
#include "control.h"

void ControlSystem::Initiate(RemoteControl *remoteControl,
		int powerSwitchChannelNo, int controlSetChannelNo)
{
	Initiate(remoteControl, remoteControl->Channels[powerSwitchChannelNo - 1],
			remoteControl->Channels[controlSetChannelNo - 1]);
}
void ControlSystem::Initiate(RemoteControl *remoteControl,
		ControlChannel *powerSwitchChannel, ControlChannel *controlSetChannel)
{
	Control = remoteControl;
	Controllers = ControlHandlers;
	powerON = false;
	controlSet = 0;
	PowerSwitch = powerSwitchChannel;
	ControlSet = controlSetChannel;
	attachController(0);
}

void ControlSystem::handleControl(uint8_t controlSetId)
{
	if (ControlSet->available)
	{
		uint8_t _controlSet = (uint8_t) (((ControlSet->controlValue + 10) * 2)
				/ 100);
		if (controlSet != _controlSet)
			controlSet = _controlSet;
	}

	if (PowerSwitch->available)
	{
		uint8_t _powerON = (uint8_t) ((PowerSwitch->controlValue + 10) / 100);
		if (powerON != _powerON)
		{
			powerON = _powerON;
			if (!_powerON)
			{
				for (int i = 0; i < 6; i++)
					Controllers->Get(i)->handleControlStop();
			}
		}
	}
}
void ControlSystem::handleControlStop()
{
	powerON = false;
}

void ControlSystem::InvokeControlHandlers()
{
	if (!powerON)
	{
		Controllers->Get(0)->handleControl(controlSet);
	}
	else
	{
		for (int i = 0; i < 6; i++)
			Controllers->Get(i)->handleControl(controlSet);
	}
}

ControlSystemCollection *ControlSystems = new ControlSystemCollection();
