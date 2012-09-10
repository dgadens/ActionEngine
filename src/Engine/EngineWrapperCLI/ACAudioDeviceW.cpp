#include "StdAfx.h"
#include "ACAudioDeviceW.h"

using namespace EngineWrapper;

ACAudioDeviceW::ACAudioDeviceW(ACAudioDevice* device)
{
	pAudioDevice = device;
}

void ACAudioDeviceW::Init(IntPtr windowPtr, bool log)
{
	pAudioDevice->Init((HWND)windowPtr.ToPointer(), log ? 1 : 0);
}