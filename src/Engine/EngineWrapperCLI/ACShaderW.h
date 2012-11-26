#pragma once

#include "ACShaders.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	public ref class ACVertexShaderW
	{
	public:
		ACVertexShader* Value;

		ACVertexShaderW();
		~ACVertexShaderW();

		void Release();
	};

	public ref class ACPixelShaderW
	{
	public:
		ACPixelShader* Value;

		ACPixelShaderW();
		~ACPixelShaderW();

		void Release();
	};

	public ref class ACGeometryShaderW
	{
	public:
		ACGeometryShader* Value;

		ACGeometryShaderW();
		~ACGeometryShaderW();

		void Release();
	};
}