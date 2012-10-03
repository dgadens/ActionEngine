#pragma once

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix.h"

namespace EngineWrapper
{
	public ref class MathConverter
	{
	public:
		static Vector4 ToUnmanaged(EngineMathCLI::Vector4^ value);
		static Vector3 ToUnmanaged(EngineMathCLI::Vector3^ value);
		static Vector2 ToUnmanaged(EngineMathCLI::Vector2^ value);
		static Matrix ToUnmanaged(EngineMathCLI::Matrix^ value);
	};
}