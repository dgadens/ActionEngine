#pragma once

#include "ACNetworkDevice.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	public ref class ACNetworkDeviceW
	{
	private:
		ACNetworkDevice* pNetworkDevice;

	public:
		ACNetworkDeviceW(ACNetworkDevice* device);
		~ACNetworkDeviceW();

		void Release();
	};
}
