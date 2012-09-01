#pragma once

#include "ACRenderDevice.h"

public ref class ACRenderDeviceW
{
private:
	ACRenderDevice* pRenderDevice;
public:
	ACRenderDeviceW(ACRenderDevice* device);
};

