#include "ACInput.h"

ACInput::ACInput(HINSTANCE hInst) 
{
   mhInst		= hInst;
   mpDevice		= nullptr;
   mpKeyboard	= nullptr;
   mpMouse		= nullptr;
   mhDLL		= nullptr;
};

ACInput::~ACInput(void) 
{ 
   Release(); 
};


//Carrega a dll requerida
HRESULT ACInput::CreateDevice(const std::string& inputLibraryName) 
{
	// carrega a dll com a implementacao da interface
	mhDLL = LoadLibraryExA(inputLibraryName.c_str(),nullptr,0);
	if(!mhDLL) 
	{
		std::string message = "Loading ";
		message.append(inputLibraryName);
		message.append(" from lib failed.");
		MessageBoxA(nullptr, message.c_str(), "ACEngine - error", MB_OK | MB_ICONERROR);
		return E_FAIL;
    }
   
	CREATEINPUTDEVICE _CreateInputDevice = 0;
	HRESULT hr;
   
	// ponteiro para a funciona da dll 'CreateInputDevice' function
	_CreateInputDevice = (CREATEINPUTDEVICE) GetProcAddress(mhDLL, "CreateInputDevice");

	// call dll create function
	hr = _CreateInputDevice(mhDLL, &mpDevice);
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "CreateInputDevice() from lib failed.", "ACEngine - error", MB_OK | MB_ICONERROR);
		mpDevice = nullptr;
		return E_FAIL;
	}
   
	return S_OK;
}; 

void ACInput::CreateKeyboard()
{
	if (mpKeyboard == nullptr)
		mpKeyboard = new ACKeyboard();
};
	
void ACInput::CreateMouse()
{
	if (mpMouse == nullptr)
		mpMouse = new ACMouse();
};

void ACInput::Release(void) 
{
	RELEASEINPUTDEVICE _ReleaseInputDevice = 0;
	HRESULT hr;
   
	if (mhDLL) 
	{
		// function pointer to dll 'ReleaseInputDevice' function
		_ReleaseInputDevice = (RELEASEINPUTDEVICE)GetProcAddress(mhDLL, "ReleaseInputDevice");
	}

	// call dll's release function
	if (mpDevice) 
	{
		hr = _ReleaseInputDevice(&mpDevice);
		if(FAILED(hr))
		{
			mpDevice = nullptr;
		}
	}

	SAFE_DELETE(mpKeyboard);

	SAFE_DELETE(mpMouse);
};