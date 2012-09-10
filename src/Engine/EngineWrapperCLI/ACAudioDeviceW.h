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

			void Init(IntPtr windowPtr, bool log);
	};
}
