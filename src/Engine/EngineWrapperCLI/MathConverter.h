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

		static Vector4* ToUnmanaged(UINT numberOfElements, array<EngineMathCLI::Vector4>^ value);
		static Vector3* ToUnmanaged(UINT numberOfElements, array<EngineMathCLI::Vector3>^ value);
		static Vector2* ToUnmanaged(UINT numberOfElements, array<EngineMathCLI::Vector2>^ value);
		static Matrix* ToUnmanaged(UINT numberOfElements, array<EngineMathCLI::Matrix>^ value);

  		static EngineMathCLI::Vector4 ToManaged(Vector4* value);
		static EngineMathCLI::Vector3 ToManaged(Vector3* value);
		static EngineMathCLI::Vector2 ToManaged(Vector2* value);
		static EngineMathCLI::Matrix ToManaged(Matrix* value);

		static array<EngineMathCLI::Vector4>^ ToManaged(UINT numberOfElements, Vector4* value);
		static array<EngineMathCLI::Vector3>^ ToManaged(UINT numberOfElements, Vector3* value);
		static array<EngineMathCLI::Vector2>^ ToManaged(UINT numberOfElements, Vector2* value);
		static array<EngineMathCLI::Matrix>^ ToManaged(UINT numberOfElements, Matrix* value);

	};
}