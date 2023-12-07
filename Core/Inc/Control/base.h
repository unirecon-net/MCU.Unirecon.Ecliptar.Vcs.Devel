/*
 * ecliptar_steering.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef _CONTROL_BASE_H_
#define _CONTROL_BASE_H_

#include <inttypes.h>
#include <iostream>

#include "drive.h"
#include "rc.h"

typedef enum
{
  left = -1, right = 1
} MixDirection;

class ControlHandler
{
  public:

  virtual void handleControl(uint8_t controlSetId) = 0;
  virtual void handleControlStop() = 0;
  void          attachController(int32_t controllerId);
  int32_t 		ControllerId = -1;

};

class ControlHandlerCollection
{
public:

	ControlHandlerCollection()
	{
		for(int i = 0; i < 17; i++)
			Table[i] = NULL;
	}
	ControlHandler* 	Table[17];
	uint8_t			Count = 0;
	void			   	Add(ControlHandler* controller, uint32_t id)
	{
		Table[id] = controller;
	}
	ControlHandler*	Get(uint32_t id)
	{
		return Table[id];
	}
	uint8_t				Remove(uint32_t id)
	{
		if(Table[id] != NULL)
		{
			Table[id] = NULL;
			return 1;
		}
		return 0;
	}
};

extern ControlHandlerCollection       *ControlHandlers;



#endif /* _CONTROL_BASE_H_ */
