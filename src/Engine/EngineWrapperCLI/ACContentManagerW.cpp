#include "StdAfx.h"
#include "ACContentManagerW.h"
#include "ACRenderDeviceW.h"
#include "ACAudioDeviceW.h"

using namespace EngineWrapper;

ACContentManagerW::ACContentManagerW(ACRenderDeviceW^ renderDevice, ACAudioDeviceW^ audioDevice)
{
	_renderDevice = renderDevice;
	_audioDevice = audioDevice;

	Value = new ACContentManager(_renderDevice->Value, _audioDevice->Value);
}

ACContentManagerW::~ACContentManagerW()
{
	Release();
}

void ACContentManagerW::Release()
{
	Value->Release();
}
