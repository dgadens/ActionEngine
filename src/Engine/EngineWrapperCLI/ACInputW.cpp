#include "StdAfx.h"
#include "ACInputW.h"

using namespace EngineWrapper;

ACInputW::ACInputW(IntPtr hInst)
{
	pInput = new ACInput((HINSTANCE)hInst.ToPointer());
}

ACInputW::~ACInputW()
{
	Release();
}

void ACInputW::Release()
{
	pInput->Release();
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

void ACInputW::CreateMouse()
{
	pInput->CreateMouse();
	_mouse = gcnew ACMouseW(pInput->GetMouse());
}

ACMouseW^ ACInputW::GetMouse()
{
	return _mouse;
}


void ACInputW::CreateKeyboard()
{
	pInput->CreateKeyboard();
	_keyboard = gcnew ACKeyboardW(pInput->GetKeyboard());
}

ACKeyboardW^ ACInputW::GetKeyboard()
{
	return _keyboard;
}