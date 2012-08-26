#pragma once

#include <msclr\marshal_cppstd.h>
#include "EngineMain.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class ACRendererW
	{
	public:
		ACRendererW(IntPtr hInst);
		HRESULT CreateDevice(String^ graphicsLibraryName);
	private:
		ACRenderer* pRenderer;
	};
}