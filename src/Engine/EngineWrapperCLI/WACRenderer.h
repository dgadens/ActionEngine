#pragma once

#include "EngineMain.h"

using namespace System;
using namespace System::Runtime::InteropServices;

ref class WACRenderer
{
public:
	WACRenderer(IntPtr hInst);
private:
	ACRenderer* pRenderer;
};

