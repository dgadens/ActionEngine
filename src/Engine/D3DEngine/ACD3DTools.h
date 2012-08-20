
#ifndef __ACD3DTOOLS_H
#define __ACD3DTOOLS_H

#include <D3D11.h>  

#include "ACD3DGlobals.h"

class ACD3DTools
{
public:
	static IDXGIFactory* GetDXGIFactory();
	static IDXGIAdapter* GetDXGIAdapter();
	static IDXGIDevice* GetDXGIDevice();
};

inline IDXGIFactory* ACD3DTools::GetDXGIFactory()
{
	//pega o factory
	IDXGIAdapter* pDXGIAdapter = GetDXGIAdapter();
	if (pDXGIAdapter!=nullptr)
	{
		IDXGIFactory* pDXGIFactory;
		HRESULT hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory);
		if ( FAILED(hr) )
		{
			MessageBoxA(nullptr, "Erro ao resgatar fabrica GXDI", "Erro", MB_OK);
			return nullptr;
		}
		else
		{
			pDXGIAdapter->Release();
			pDXGIAdapter = nullptr;
			return pDXGIFactory;
		}
	}

	return nullptr;
};

inline IDXGIAdapter* ACD3DTools::GetDXGIAdapter()
{
	//pega o adaptador
	IDXGIDevice* pDXGIDevice = GetDXGIDevice();
	if (pDXGIDevice!=nullptr)
	{
		IDXGIAdapter* pDXGIAdapter;
		HRESULT hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
		if ( FAILED(hr) )
		{
			MessageBoxA(nullptr, "Erro ao resgatar adaptador GXDI", "Erro", MB_OK);
			return nullptr;
		}
		else
		{
			pDXGIDevice->Release();
			pDXGIDevice = nullptr;
			return pDXGIAdapter;
		}
	}

	return nullptr;
};

inline IDXGIDevice* ACD3DTools::GetDXGIDevice()
{
	//pega o dispositivo dxgi 
	if (ACD3DGlobals::G_pD3dDevice != nullptr)
	{
		IDXGIDevice* pDXGIDevice;
		HRESULT hr = ACD3DGlobals::G_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
		if ( FAILED(hr) )
		{
			MessageBoxA(nullptr, "Erro ao resgatar dispositivo GXDI", "Erro", MB_OK);
			return nullptr;
		}
		else
			return pDXGIDevice;
	}

	return nullptr;
};

#endif