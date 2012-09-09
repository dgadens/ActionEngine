#include "StdAfx.h"
#include "ACRenderDeviceW.h"


ACRenderDeviceW::ACRenderDeviceW(ACRenderDevice* device)
{
	pRenderDevice = device;
}

void ACRenderDeviceW::Init(IntPtr windowPtr, bool enableVSync, bool log)
{
	pRenderDevice->Init((HWND)windowPtr.ToPointer(), enableVSync ? 1 : 0, log ? 1 : 0);
}

void ACRenderDeviceW::SetActiveViewport(IntPtr windowPtr)
{
	pRenderDevice->SetActiveViewport((HWND)windowPtr.ToPointer());
}