#include "rc.h"

RemoteControl::RemoteControl(RemoteController *rc)
{
	this->rc = rc;
	Channels = rc->Channels;
}

u8 RemoteControl::Attached()
{
	return rc->Attached();
}

u8 RemoteControl::Available()
{
	return rc->Available();
}

ControlChannel** RemoteControl::ReadChannels()
{
	return rc->ReadChannels();
}

void RemoteControl::PrintInfoReport(ControlChannel **channels)
{
	rc->PrintInfoReport(channels);
}
