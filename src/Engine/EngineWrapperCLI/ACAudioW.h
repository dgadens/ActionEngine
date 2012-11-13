#pragma once

#include <msclr\marshal_cppstd.h>
#include "EngineMain.h"
#include "ACAudioDeviceW.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class ACAudioW
	{
	private:
		ACAudio* pAudio;
		ACAudioDeviceW^ _audioDevice;

	public:
		ACAudioW(IntPtr hInst);
		~ACAudioW();

		HRESULT CreateDevice(String^ audioLibraryName);
		ACAudioDeviceW^ GetDevice();
		void Release();
	};
}