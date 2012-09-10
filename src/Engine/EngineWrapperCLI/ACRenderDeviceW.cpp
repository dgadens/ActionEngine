#include "StdAfx.h"
#include "ACRenderDeviceW.h"

using namespace EngineWrapper;

ACRenderDeviceW::ACRenderDeviceW(ACRenderDevice* device)
{
	Value = device;
}

void ACRenderDeviceW::Init(IntPtr windowPtr, bool enableVSync, bool log)
{
	Value->Init((HWND)windowPtr.ToPointer(), enableVSync ? 1 : 0, log ? 1 : 0);
}

void ACRenderDeviceW::SetActiveViewport(IntPtr windowPtr)
{
	Value->SetActiveViewport((HWND)windowPtr.ToPointer());
}