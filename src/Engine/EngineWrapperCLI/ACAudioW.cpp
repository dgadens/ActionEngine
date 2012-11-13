#include "StdAfx.h"
#include "ACAudioW.h"

using namespace EngineWrapper;

ACAudioW::ACAudioW(IntPtr hInst)
{
	pAudio = new ACAudio((HINSTANCE)hInst.ToPointer());
}

ACAudioW::~ACAudioW()
{
	Release();
}

void ACAudioW::Release()
{
	pAudio->Release();
}

HRESULT ACAudioW::CreateDevice(String^ audioLibraryName)
{
	HRESULT hr = pAudio->CreateDevice(marshal_as<std::string>(audioLibraryName));
	_audioDevice = gcnew ACAudioDeviceW(pAudio->GetDevice());
	return hr;
}

ACAudioDeviceW^ ACAudioW::GetDevice()
{
	return _audioDevice;
}

