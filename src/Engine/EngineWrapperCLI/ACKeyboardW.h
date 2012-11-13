#pragma once

#include "EngineMain.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	public ref class ACKeyboardW
	{
	private:
		ACKeyboard* pKeyboard;
	public:
		ACKeyboardW(ACKeyboard* keyboard);
		~ACKeyboardW();

		void Release();
	};
}