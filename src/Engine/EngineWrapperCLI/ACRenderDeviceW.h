#pragma once

#include "ACRenderDevice.h"

using namespace System;
using namespace System::Runtime::InteropServices;

public ref class ACRenderDeviceW
{
private:
	ACRenderDevice* pRenderDevice;
public:
	ACRenderDeviceW(ACRenderDevice* device);
	void Init(IntPtr windowPtr, bool enableVSync, bool log);
	void SetActiveViewport(IntPtr windowPtr);
};

