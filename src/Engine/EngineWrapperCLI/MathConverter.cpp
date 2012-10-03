#include "StdAfx.h"
#include "MathConverter.h"

using namespace EngineWrapper;

Vector4 MathConverter::ToUnmanaged(EngineMathCLI::Vector4^ value)
{
	return Vector4(value->X, value->Y, value->Z, value->W);
}

Vector3 MathConverter::ToUnmanaged(EngineMathCLI::Vector3^ value)
{
	return Vector3(value->X, value->Y, value->Z);
}

Vector2 MathConverter::ToUnmanaged(EngineMathCLI::Vector2^ value)
{
	return Vector2(value->X, value->Y);
}

Matrix MathConverter::ToUnmanaged(EngineMathCLI::Matrix^ value)
{
	return Matrix(
		value->M11, value->M12, value->M13, value->M14,
		value->M21, value->M22, value->M23, value->M24,
		value->M31, value->M32, value->M33, value->M34,
		value->M41, value->M42, value->M43, value->M44);
}