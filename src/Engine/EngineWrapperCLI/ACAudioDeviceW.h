#pragma once

#include "ACAudioDevice.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	public ref class ACAudioDeviceW
	{
	public:
		ACAudioDevice* Value;

		ACAudioDeviceW(ACAudioDevice* device);
		~ACAudioDeviceW();

		void Init(IntPtr windowPtr, bool log);
		void SetListener(EngineMathCLI::Vector3^ position, EngineMathCLI::Vector3^ direction, EngineMathCLI::Vector3^ up, EngineMathCLI::Vector3^ velocity);

		void Release();
	};
}
