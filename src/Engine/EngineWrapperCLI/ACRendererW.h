#pragma once

#include "EngineMain.h"

using namespace System;
using namespace System::Runtime::InteropServices;

ref class ACRendererW
{
public:
	ACRendererW(IntPtr hInst);
private:
	ACRenderer* pRenderer;
};

