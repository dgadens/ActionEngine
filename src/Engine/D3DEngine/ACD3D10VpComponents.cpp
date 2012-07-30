#include "ACD3D10VpComponents.h"

ACD3D10VpComponents::ACD3D10VpComponents()
{
	pSwapChain = nullptr; 
	pRenderTargetView = nullptr; 
	pDepthStencilView = nullptr; 
};

ACD3D10VpComponents::~ACD3D10VpComponents()
{
	Release();
};

void ACD3D10VpComponents::Release()
{
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pSwapChain); 
};