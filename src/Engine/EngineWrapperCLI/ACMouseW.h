#pragma once

#include "EngineMain.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	public ref class ACMouseW
	{
		private:
			ACMouse* pMouse;
		public:
			ACMouseW(ACMouse* mouse);

			/*void SetButtonDown(ACMOUSEBUTTON button);
			void SetButtonUp(ACMOUSEBUTTON button);
			void SetPosition(int x, int y);

			void GetState(ACMouseState* mouseState);*/
	};
}