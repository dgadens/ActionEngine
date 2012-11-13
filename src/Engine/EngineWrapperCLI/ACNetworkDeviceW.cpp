#include "StdAfx.h"
#include "ACNetworkDeviceW.h"

using namespace EngineWrapper;

ACNetworkDeviceW::ACNetworkDeviceW(ACNetworkDevice* device)
{
	pNetworkDevice = device;
}

ACNetworkDeviceW::~ACNetworkDeviceW()
{
	Release();
}

void ACNetworkDeviceW::Release()
{
	pNetworkDevice->Release();
}
