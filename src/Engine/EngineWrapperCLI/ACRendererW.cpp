#include "StdAfx.h"
#include "ACRendererW.h"

using namespace EngineWrapper;

ACRendererW::ACRendererW(IntPtr hInst)
{
	pRenderer = new ACRenderer((HINSTANCE)hInst.ToPointer());
};

HRESULT ACRendererW::CreateDevice(String^ graphicsLibraryName)
{
	HRESULT hr = pRenderer->CreateDevice(marshal_as<std::string>(graphicsLibraryName));
	_renderDevice = gcnew ACRenderDeviceW(pRenderer->GetDevice());
	return hr;
};

ACRenderDeviceW^ ACRendererW::GetDevice()
{
	return _renderDevice;
};