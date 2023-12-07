/*
 * ecliptar_rc_ibus.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef RC_BASE_H
#define RC_BASE_H

#include <stdio.h>
#include <inttypes.h>
#include <system.h>
#include "Control/channel.h"


class RemoteController // @suppress("Class has a virtual method and non-virtual destructor")
{
public:

	virtual u8					 Attached() = 0;

	virtual ControlChannel**   ReadChannels() = 0;

	virtual u8            		 Available() = 0;

	virtual void           	 PrintInfoReport(ControlChannel** channels) = 0;

	ControlChannel**   	Channels;

};


#endif /* RC_IBUS_H */
