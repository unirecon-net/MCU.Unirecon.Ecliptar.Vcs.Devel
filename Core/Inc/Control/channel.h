/*
 * ecliptar_rc_ibus.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdio.h>
#include <inttypes.h>
#include <system.h>

struct ControlChannel
{
  volatile i32 			      	index;
  volatile f32          	  		controlValue;
  volatile uint16_t      	       	minPulseWidth;
  volatile uint16_t        		maxPulseWidth;
  volatile uint16_t           	midPulseWidth;
  volatile i32                   	signalLostPosition;
  volatile uint16_t       		innerPulseWidth;
  volatile uint16_t       		pulseWidth;
  volatile int16_t         		pulseDelta;
  volatile uint8_t         		available :1;
};


#endif /* CHANNEL_H */
