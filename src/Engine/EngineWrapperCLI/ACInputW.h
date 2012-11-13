#pragma once

#include <msclr\marshal_cppstd.h>
#include "EngineMain.h"
#include "ACInputDeviceW.h"
#include "ACMouseW.h"
#include "ACKeyboardW.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class ACInputW
	{
	private:
		ACInput* pInput;
		ACInputDeviceW^ _inputDevice;
		ACMouseW^ _mouse;
		ACKeyboardW^ _keyboard;

	public:
		ACInputW(IntPtr hInst);
		~ACInputW();

		HRESULT CreateDevice(String^ inputLibraryName);
		ACInputDeviceW^ GetDevice();

		void CreateMouse();
		ACMouseW^ GetMouse();

		void CreateKeyboard();
		ACKeyboardW^ GetKeyboard();

		void Release();
	};
}