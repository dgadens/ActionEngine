
#ifndef __ACD3D10TOOLS_H
#define __ACD3D10TOOLS_H

#include <D3D10.h>  

#include "ACD3D10Globals.h"

class ACD3D10Tools
{
public:
	static IDXGIFactory* GetDXGIFactory();
	static IDXGIAdapter* GetDXGIAdapter();
	static IDXGIDevice* GetDXGIDevice();
};

inline IDXGIFactory* ACD3D10Tools::GetDXGIFactory()
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

inline IDXGIAdapter* ACD3D10Tools::GetDXGIAdapter()
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

inline IDXGIDevice* ACD3D10Tools::GetDXGIDevice()
{
	//pega o dispositivo dxgi 
	if (ACD3D10Globals::G_pD3dDevice != nullptr)
	{
		IDXGIDevice* pDXGIDevice;
		HRESULT hr = ACD3D10Globals::G_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
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