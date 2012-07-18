#include "ACDIDevice.h"

void ACDIDevice::Create(LPDIRECTINPUT8 pDI, HWND hWnd) 
{
   mhWnd    = hWnd;
   mpDI     = pDI;
   mpDevice = nullptr;
};

void ACDIDevice::Release(void) 
{
	if (mpDevice) 
	{
		mpDevice->Unacquire();
		mpDevice->Release();
		mpDevice = nullptr;
	}
};

//chama tudo q o directinput precisa para inicializar os objetos
HRESULT ACDIDevice::CrankUp(REFGUID rguid, LPCDIDATAFORMAT pdf) 
{
	DWORD dwFlags = DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;

	// if device is already build destroy it
	if (mpDevice) 
	{
		mpDevice->Unacquire();
		mpDevice->Release();
		mpDevice = nullptr;
    }

	// 1. Step: cria o dispositivo
	if ( FAILED(mpDI->CreateDevice(rguid, &mpDevice, nullptr))) 
	{
		MessageBoxA(nullptr," Erro criando dispositivo.", "ACDI error", MB_OK);
		return AC_FAIL; 
    } 

	// set the correct device data format
	if ( FAILED(mpDevice->SetDataFormat(pdf))) 
	{
		MessageBoxA(nullptr," Erro setando o formato de dados.", "ACDI error", MB_OK);
		return AC_FAIL;
	}

	// set the cooperation level with windows
	if (FAILED(mpDevice->SetCooperativeLevel( mhWnd, dwFlags)))
	{
		MessageBoxA(NULL," Erro setando o level de cooperacao com o windows.", "ACDI error", MB_OK);
		return AC_FAIL;
	}

	return AC_OK;
};

/**
 * Get the state or data from the device object.
 * -> IN: InputDevType - keyboard, mouse or joystick
 *        void*        - stores the data
 *        DWORD*       - stores a counter (mouse only)
 */
HRESULT ACDIDevice::GetData(InputDevType Type, void *pData, DWORD *pdwNum) 
{
	HRESULT hr= AC_FAIL;
	size_t size=0;
	size = sizeof(DIJOYSTATE);
	hr = mpDevice->GetDeviceState(size, pData);

	if (FAILED(hr)) 
	{
		// if lost or not yet acquired then acquire it at all costs
		if ( (hr==DIERR_NOTACQUIRED) || (hr==DIERR_INPUTLOST) ) 
		{
			hr = mpDevice->Acquire();
			while (hr==DIERR_INPUTLOST)
			hr = mpDevice->Acquire();
         
			// if another application is using this input device
			// we have to give up and try next frame
			if (hr==DIERR_OTHERAPPHASPRIO) return AC_OK;
         
			// if we got back device then try again to read data
			if (SUCCEEDED(hr)) 
				hr = mpDevice->GetDeviceState(size, pData);

			if (FAILED(hr)) 
				return AC_FAIL;
		}
		else 
			return AC_FAIL;
	}
	return AC_OK;

};