/*
 * main.h
 *
 *  Created on: 16 kwi 2021
 *      Author: merlin
 */

#include "ecliptar.h"

int main(void)
{
	System_Config();

	for(;;)
	{
		RC.PrintInfoReport(RC.ReadChannels());

		Drive.PrintMeasureReport();
	}
}

