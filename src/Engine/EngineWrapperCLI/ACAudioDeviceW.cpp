#include "StdAfx.h"
#include "ACAudioDeviceW.h"

using namespace EngineWrapper;

ACAudioDeviceW::ACAudioDeviceW(ACAudioDevice* device)
{
	Value = device;
}

void ACAudioDeviceW::Init(IntPtr windowPtr, bool log)
{
	Value->Init((HWND)windowPtr.ToPointer(), log ? 1 : 0);
}