#include "ACNetwork.h"

ACNetwork::ACNetwork(HINSTANCE hInst) 
{
   mhInst   = hInst;
   mpDevice = nullptr;
   mhDLL    = nullptr;
};

ACNetwork::~ACNetwork(void) 
{ 
   Release(); 
};

HRESULT ACNetwork::CreateDevice(const std::string &mplayerLibraryName) 
{
	mhDLL = LoadLibraryA(mplayerLibraryName.c_str());
	if(!mhDLL) 
	{
		std::string message = "Loading ";
		message.append(mplayerLibraryName);
		message.append(" from lib failed.");
		MessageBoxA(nullptr,message.c_str(), "ACEngine - error", MB_OK | MB_ICONERROR);
		return E_FAIL;
    }
   
	CREATENETWORKDEVICE _CreateNetworkDevice = 0;
	HRESULT hr;
   
	// function pointer to dll's 'CreateNetworkDevice' function
	_CreateNetworkDevice = (CREATENETWORKDEVICE)GetProcAddress(mhDLL,"CreateNetworkDevice");
	// call dll's create function
	hr = _CreateNetworkDevice(mhDLL, &mpDevice);

	if(FAILED(hr))
	{
		MessageBox(nullptr,L"CreateNetworkDevice() from lib failed.", L"ACEngine - error", MB_OK | MB_ICONERROR);
		mpDevice = nullptr;
		return E_FAIL;
	}
   
	return S_OK;
};

void ACNetwork::Release(void) 
{
   RELEASENETWORKDEVICE _ReleaseNetworkDevice = 0;
   HRESULT hr;
   
	if (mhDLL) 
	{
		// function pointer to dll 'ReleaseNetworkDevice' function
		_ReleaseNetworkDevice = (RELEASENETWORKDEVICE)GetProcAddress(mhDLL, "ReleaseNetworkDevice");
    }

	// call dll's release function
	if (mpDevice) 
	{
		hr = _ReleaseNetworkDevice(&mpDevice);
		if(FAILED(hr))
		{
			mpDevice = nullptr;
        }
	}
}