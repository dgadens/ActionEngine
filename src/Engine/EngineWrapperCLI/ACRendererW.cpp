#include "StdAfx.h"
#include "ACRendererW.h"

using namespace EngineWrapper;

ACRendererW::ACRendererW(IntPtr hInst)
{
	pRenderer = new ACRenderer((HINSTANCE)hInst.ToPointer());
};

HRESULT ACRendererW::CreateDevice(String^ graphicsLibraryName)
{
	return pRenderer->CreateDevice(marshal_as<std::string>(graphicsLibraryName));
};