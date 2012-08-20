//*************************
// estrutura q mantem os objetos de cada vp
// creator: Daniel Gadens
// date: 24/06/2011
//**************************

#ifndef __ACD3DVPCOMPONENTS_H
#define __ACD3DVPCOMPONENTS_H

#include <d3d11.h>
#include <DXGI.h>

#include "GenericTools.h"

struct ACD3DVpComponents
{
	IDXGISwapChain*				pSwapChain; 
	ID3D11RenderTargetView*		pRenderTargetView; 
	ID3D11DepthStencilView*		pDepthStencilView; 
	D3D11_VIEWPORT 				Viewport;

	ACD3DVpComponents() 
	{
		pSwapChain = nullptr; 
		pRenderTargetView = nullptr; 
		pDepthStencilView = nullptr; 

		ZeroMemory(&Viewport, sizeof ( D3D11_VIEWPORT ));
	};

	~ACD3DVpComponents()
	{
		Release();
	};

	void Release()
	{
		SAFE_RELEASE(pRenderTargetView);
		SAFE_RELEASE(pDepthStencilView);
		SAFE_RELEASE(pSwapChain); 
	};
};

#endif