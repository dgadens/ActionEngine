#pragma once

#include "Vector3.h"

namespace EngineWrapper
{
	public ref class MathConverter
	{
	public:
		static Vector3 ToUnmanaged(EngineMathCLI::Vector3^ value);
	};
}