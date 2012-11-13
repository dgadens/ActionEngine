#include "StdAfx.h"
#include "ACNetworkW.h"

using namespace EngineWrapper;

ACNetworkW::ACNetworkW(IntPtr hInst)
{
	pNetwork = new ACNetwork((HINSTANCE)hInst.ToPointer());
}

ACNetworkW::~ACNetworkW()
{
	Release();
}

void ACNetworkW::Release()
{
	pNetwork->Release();
}

HRESULT ACNetworkW::CreateDevice(String^ networkLibraryName)
{
	HRESULT hr = pNetwork->CreateDevice(marshal_as<std::string>(networkLibraryName));
	_networkDevice = gcnew ACNetworkDeviceW(pNetwork->GetDevice());
	return hr;
}

ACNetworkDeviceW^ ACNetworkW::GetDevice()
{
	return _networkDevice;
}