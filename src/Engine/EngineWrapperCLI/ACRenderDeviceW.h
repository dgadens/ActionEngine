#pragma once

#include "ACRenderDevice.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
public ref class ACRenderDeviceW
{
	public:
		ACRenderDevice* Value;

		ACRenderDeviceW(ACRenderDevice* device);

		void Init(IntPtr windowPtr, bool enableVSync, bool log);
		void SetActiveViewport(IntPtr windowPtr);
	};
}
