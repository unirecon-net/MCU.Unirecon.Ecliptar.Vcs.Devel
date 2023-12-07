/*
 * main.h
 *
 *  Created on: 16 kwi 2021
 *      Author: merlin
 */

#include "config.h"
#include "system.h"

int main(void)
{
	System_Config();

	ControlSystem* cs0 = ControlSystems->Get(0);

	for(;;)
	{
		cs0->Control->ReadChannels();

		cs0->InvokeControlHandlers();

		Drive->PrintMeasureReport();
	}
}

