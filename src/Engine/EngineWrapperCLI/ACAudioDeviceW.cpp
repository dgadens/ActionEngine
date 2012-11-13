#include "StdAfx.h"
#include "ACAudioDeviceW.h"
#include "MathConverter.h"

using namespace EngineWrapper;

ACAudioDeviceW::ACAudioDeviceW(ACAudioDevice* device)
{
	Value = device;
}

ACAudioDeviceW::~ACAudioDeviceW()
{
	Release();
}

void ACAudioDeviceW::Release()
{
	Value->Release();
}

void ACAudioDeviceW::Init(IntPtr windowPtr, bool log)
{
	Value->Init((HWND)windowPtr.ToPointer(), log ? 1 : 0);
}

void ACAudioDeviceW::SetListener(EngineMathCLI::Vector3^ position, EngineMathCLI::Vector3^ direction, EngineMathCLI::Vector3^ up, EngineMathCLI::Vector3^ velocity)
{
	Value->SetListener(
		MathConverter::ToUnmanaged(position),
		MathConverter::ToUnmanaged(direction),
		MathConverter::ToUnmanaged(up),
		MathConverter::ToUnmanaged(velocity));
}
