#pragma once

#include <msclr\marshal_cppstd.h>
#include "ACConfigurations.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class ACConfigurationsW
	{
	public:
		static void Initialize();
	};
}

