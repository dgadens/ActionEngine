#include "StdAfx.h"
#include "ACInputDeviceW.h"

using namespace EngineWrapper;

ACInputDeviceW::ACInputDeviceW(ACInputDevice* device)
{
	pInputDevice = device;
}

ACInputDeviceW::~ACInputDeviceW()
{
	Release();
}

void ACInputDeviceW::Release()
{
	pInputDevice->Release();
}
