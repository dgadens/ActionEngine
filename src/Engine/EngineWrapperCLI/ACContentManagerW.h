#pragma once

#include "ACContentManager.h"
#include "ACRenderDeviceW.h"
#include "ACAudioDeviceW.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	public ref class ACContentManagerW
	{
	private:
		ACRenderDeviceW^ _renderDevice;
		ACAudioDeviceW^ _audioDevice;

	public:
		ACContentManager* Value;

		ACContentManagerW(ACRenderDeviceW^ renderDevice, ACAudioDeviceW^ audioDevice);
	};

}