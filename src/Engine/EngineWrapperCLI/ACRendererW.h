#pragma once

#include <msclr\marshal_cppstd.h>
#include "EngineMain.h"
#include "ACRenderDeviceW.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class ACRendererW
	{
	private:
		ACRenderer* pRenderer;
		ACRenderDeviceW^ _renderDevice;

	public:
		ACRendererW(IntPtr hInst);
		~ACRendererW();

		HRESULT CreateDevice(String^ graphicsLibraryName);
		ACRenderDeviceW^ GetDevice();

		void Release();
	};
}