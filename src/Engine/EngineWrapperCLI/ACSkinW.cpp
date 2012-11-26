#include "StdAfx.h"
#include "ACSkinW.h"
#include "ACRenderDeviceW.h"
#include "ACContentManagerW.h"

using namespace EngineWrapper;

ACSkinW::ACSkinW(ACRenderDeviceW^ gDevice, ACContentManagerW^ cManager, BOOL useMaterial)
{
	Value = new ACSkin(gDevice->Value, cManager->Value, useMaterial);
}

ACSkinW::~ACSkinW()
{
	Release();
}

void ACSkinW::Release()
{
	Value->Release();
}

void ACSkinW::SetMaterial(EngineMathCLI::Vector3^ ambientColor, EngineMathCLI::Vector4^ diffuseColor, EngineMathCLI::Vector3^ specularColor, EngineMathCLI::Vector3^ emissiveColor, float specularPower)
{
	Value->SetMaterial(MathConverter::ToUnmanaged(ambientColor),
					   MathConverter::ToUnmanaged(diffuseColor),
					   MathConverter::ToUnmanaged(specularColor),
					   MathConverter::ToUnmanaged(emissiveColor),
					   specularPower);
}

void ACSkinW::SetTexture(String^ name, BYTE index)
{
	Value->SetTexture(marshal_as<std::string>(name), index);
}
