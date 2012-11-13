#pragma once

#include "ACContentManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	ref class ACRenderDeviceW;
	ref class ACAudioDeviceW;

	public ref class ACContentManagerW
	{
	private:
		ACRenderDeviceW^ _renderDevice;
		ACAudioDeviceW^ _audioDevice;

	public:

		ACContentManager* Value;

		ACContentManagerW(ACRenderDeviceW^ renderDevice, ACAudioDeviceW^ audioDevice);
		~ACContentManagerW();

		void Release();
	};

}