#include "StdAfx.h"
#include "ACInputW.h"

using namespace EngineWrapper;

ACInputW::ACInputW(IntPtr hInst)
{
	pInput = new ACInput((HINSTANCE)hInst.ToPointer());
}

HRESULT ACInputW::CreateDevice(String^ inputLibraryName)
{
	HRESULT hr = pInput->CreateDevice(marshal_as<std::string>(inputLibraryName));
	_inputDevice = gcnew ACInputDeviceW(pInput->GetDevice());
	return hr;
}

ACInputDeviceW^ ACInputW::GetDevice()
{
	return _inputDevice;
}