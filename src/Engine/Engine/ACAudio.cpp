#include "ACAudio.h"

ACAudio::ACAudio(HINSTANCE hInst) 
{
   mhInst   = hInst;
   mpDevice = nullptr;
   mhDLL    = nullptr;
};

ACAudio::~ACAudio(void) 
{ 
   Release(); 
};

HRESULT ACAudio::CreateDevice(const std::string &audioLibraryName) 
{
	mhDLL = LoadLibraryA(audioLibraryName.c_str());
	if(!mhDLL) 
	{
		return E_FAIL;
    }
   
	CREATEAUDIODEVICE _CreateAudioDevice = 0;
	HRESULT hr;
   
	// function pointer to dll's 'CreateNetworkDevice' function
	_CreateAudioDevice = (CREATEAUDIODEVICE)GetProcAddress(mhDLL,"CreateAudioDevice");
	// call dll's create function
	hr = _CreateAudioDevice(mhDLL, &mpDevice);

	if(FAILED(hr))
	{
		mpDevice = nullptr;
		return E_FAIL;
	}
   
	return S_OK;
};

void ACAudio::Release(void) 
{
   RELEASEAUDIODEVICE _ReleaseAudioDevice = 0;
   HRESULT hr;
   
	if (mhDLL) 
	{
		// function pointer to dll 'ReleaseNetworkDevice' function
		_ReleaseAudioDevice = (RELEASEAUDIODEVICE)GetProcAddress(mhDLL, "ReleaseAudioDevice");
    }

	// call dll's release function
	if (mpDevice) 
	{
		hr = _ReleaseAudioDevice(&mpDevice);
		if(FAILED(hr))
		{
			mpDevice = nullptr;
        }
	}
}