#pragma once

#include <string.h>
#include "ACSkin.h"
#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
	ref class ACRenderDeviceW;
	ref class ACContentManagerW;

	public ref class ACSkinW
	{
	public:

		ACSkin* Value;

		ACSkinW(ACRenderDeviceW^ gDevice, ACContentManagerW^ cManager, BOOL useMaterial);
		~ACSkinW();

		void SetMaterial(EngineMathCLI::Vector3^ ambientColor, EngineMathCLI::Vector4^ diffuseColor, EngineMathCLI::Vector3^ specularColor, EngineMathCLI::Vector3^ emissiveColor, float specularPower);
		void SetTexture(String^ name, BYTE index);
		
		void Release();
	};

}