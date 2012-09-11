#include "StdAfx.h"
#include "MathConverter.h"

using namespace EngineWrapper;

Vector3 MathConverter::ToUnmanaged(EngineMathCLI::Vector3^ value)
{
	return Vector3(value->X, value->Y, value->Z);
}