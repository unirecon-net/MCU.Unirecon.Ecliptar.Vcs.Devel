/*
 * ecliptar_steering.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "Control/base.h"
#include "Control/ecliptor.h"
#include "Control/lights.h"
#include "Control/motor.h"
#include "Control/event.h"
#include "Control/fpv.h"
#include "Control/channel.h"
#include "RC/base.h"
#include "rc.h"

class ControlSystem : public ControlHandler
{
public:
	ControlSystem(RemoteControl* remoteControl, int startupChannelNo, int controlSetChannelNo)
	{
		Initiate(remoteControl, startupChannelNo, controlSetChannelNo);
	}
	ControlSystem(RemoteControl* remoteControl, ControlChannel* startupChannel, ControlChannel* controlSetChannel)
	{
		Initiate(remoteControl, startupChannel, controlSetChannel);
	}

	void Initiate(RemoteControl* remoteControl, int startupChannelNo, int controlSetChannelNo);
	void Initiate(RemoteControl* remoteControl, ControlChannel* startupChannel, ControlChannel* controlSetChannel);

	virtual void handleControl(uint8_t controlSetId);
	virtual void handleControlStop();

	void InvokeControlHandlers();


	ControlChannel *PowerSwitch;
	ControlChannel *ControlSet;

	RemoteControl* Control;

	ControlHandlerCollection *Controllers;

	uint8_t powerON :1;
	uint8_t controlSet :2;

} ;

class ControlSystemCollection
{
public:
	ControlSystemCollection()
	{
		for(int i = 0; i < 32; i++)
			Table[i] = NULL;
	}

	ControlSystem* 	Table[32];

	uint8_t Count = 0;

	void Add(ControlSystem *system)
	{
		Table[Count++] = system;
	}
	void Add(ControlSystem *system, uint32_t id)
	{
		Table[id] = system;
	}
	ControlSystem* Get(uint32_t id)
	{
		return Table[id];
	}

} ;

extern ControlSystemCollection* ControlSystems;


#endif /* _CONTROL_H_ */
