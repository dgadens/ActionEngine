//*************************
// estrutura q mantem os objetos de cada vp
// creator: Daniel Gadens
// date: 24/06/2011
//**************************

#ifndef __ACD3D10VPCOMPONENTS_H
#define __ACD3D10VPCOMPONENTS_H

#include <d3d10.h>
#include <d3dx10.h>
#include <DXGI.h>
#include "ACD3D10RenderToTexture.h"

struct ACD3D10VpComponents
{
	IDXGISwapChain*				pSwapChain; 
	ID3D10RenderTargetView*		pRenderTargetView; 
	ID3D10DepthStencilView*		pDepthStencilView; 
	D3D10_VIEWPORT 				Viewport;
	ACD3D10RenderToTexture*		pRenderToTexture;

	ACD3D10VpComponents();
	~ACD3D10VpComponents()
	{
		Release();
	};

	void Release();
};

void inline ACD3D10VpComponents::Release()
{
	SAFE_RELEASE(pSwapChain); 
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pRenderToTexture);
};

#endif