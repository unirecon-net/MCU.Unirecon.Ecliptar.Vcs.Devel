/*
 * ecliptar_steering.c
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */
#ifndef __CONTROL_EVENT_H
#define __CONTROL_EVENT_H

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

class ControlEvent
{
	virtual void	ControlInputReady() = 0;
	virtual void	ControlInputDone() = 0;

	uint8_t InputReady = 0;

	uint8_t InputDone = 0;

} ;

#endif /* ___CONTROL_EVENT_H */
