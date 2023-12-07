/*
 * ecliptar_rc_ibus.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef _RC_H
#define _RC_H

#include <stdio.h>
#include <inttypes.h>
#include <usart.h>

#include "RC/base.h"
#include "RC/ibus.h"

class RemoteControl : public RemoteController
{
public:
	RemoteControl(RemoteController* rc);

	virtual u8            	Attached();

	virtual u8            	Available();

	virtual ControlChannel**   	ReadChannels();

	virtual void           		PrintInfoReport(ControlChannel** channels);

	RemoteController* rc;

	ControlChannel**   	Channels;

};



#endif /* _RC_H */
