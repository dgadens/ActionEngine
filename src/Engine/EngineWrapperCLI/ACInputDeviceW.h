#pragma once

#include "ACInputDevice.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	public ref class ACInputDeviceW
	{
		private:
			ACInputDevice* pInputDevice;
		public:
			ACInputDeviceW(ACInputDevice* device);
	};
}
