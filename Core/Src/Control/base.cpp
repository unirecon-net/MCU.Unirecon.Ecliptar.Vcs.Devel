/*
 * Control/base.c
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#include "Control/base.h"

void        ControlHandler::attachController(int32_t id)
{
    ControlHandlers->Add(this, id);
    ControllerId = id;
}

ControlHandlerCollection* ControlHandlers = new ControlHandlerCollection();

