#pragma once

#include <msclr\marshal_cppstd.h>
#include "EngineMain.h"
#include "ACNetworkDeviceW.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class ACNetworkW
	{
		private:
			ACNetwork* pNetwork;
			ACNetworkDeviceW^ _networkDevice;
		public:
			ACNetworkW(IntPtr hInst);
			HRESULT CreateDevice(String^ networkLibraryName);
			ACNetworkDeviceW^ GetDevice();
	};
}