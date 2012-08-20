
#ifndef __ACD3DCONFIGURATIONS_H
#define __ACD3DCONFIGURATIONS_H

#include <d3d11.h>
#include <DXGI.h>

#include "EngineCommon.h"
#include "ACD3DGlobals.h"

class ACD3DConfigurations
{
	//rasterization
	static ID3D11RasterizerState* pCurrentRasterState;
	static ID3D11RasterizerState* pRasterStateSolidCullCCW;
	static ID3D11RasterizerState* pRasterStateWireFrameCullCCW;
	static ID3D11RasterizerState* pRasterStateSolidCullCW;
	static ID3D11RasterizerState* pRasterStateWireFrameCullCW;
	static ID3D11RasterizerState* pRasterStateSolidCullNone;
	static ID3D11RasterizerState* pRasterStateWireFrameCullNone;

	//depth state
	static ID3D11DepthStencilState* pCurrentDepthStencilState;
	static ID3D11DepthStencilState* pDepthStencilStateEnableDepthBuffer;
	static ID3D11DepthStencilState* pDepthStencilStateDisableDepthBuffer;

	//blend state
	static ID3D11BlendState* pCurrentBlendState;
	static ID3D11BlendState* pBlendStateDefault;
	static ID3D11BlendState* pBlendStateAdditive;
	static ID3D11BlendState* pBlendStateNonPremultiplied;
	static ID3D11BlendState* pBlendStateAlphaBlend;
	static ID3D11BlendState* pBlendStateOpaque;

	//sampler state
	static ID3D11SamplerState* pCurrentSamplerState[16];
	static ID3D11SamplerState* pSamplerStatePointWrap;
	static ID3D11SamplerState* pSamplerStateBilinearWrap;
	static ID3D11SamplerState* pSamplerStateTrilinearWrap;
	static ID3D11SamplerState* pSamplerStateAnisotropicWrap;
	static ID3D11SamplerState* pSamplerStatePointClamp;
	static ID3D11SamplerState* pSamplerStateBilinearClamp;
	static ID3D11SamplerState* pSamplerStateTrilinearClamp;
	static ID3D11SamplerState* pSamplerStateAnisotropicClamp;
	static ID3D11SamplerState* pSamplerStatePointBorder;
	static ID3D11SamplerState* pSamplerStateBilinearBorder;
	static ID3D11SamplerState* pSamplerStateTrilinearBorder;
	static ID3D11SamplerState* pSamplerStateAnisotropicBorder;

public:
	static DXGI_SWAP_CHAIN_DESC DefineSwapShain(const HWND hWnd, UINT width, UINT height, UINT numerator, UINT denominator);
	static HRESULT DefineDepthStencilView(ID3D11Device *pDevice, ID3D11Texture2D** pDepthStencil, ID3D11DepthStencilView** pDepthStencilView, UINT width, UINT height);
	
	//rasterization state
	static void DefineRasterizeStateSolidCullCCW();
	static void DefineRasterizeStateWireframeCullCCW();
	static void DefineRasterizeStateSolidCullCW();
	static void DefineRasterizeStateWireframeCullCW();
	static void DefineRasterizeStateSolidCullNone();
	static void DefineRasterizeStateWireframeCullNone();
	
	//depth state
	static void DefineDepthStencilStateEnableDepthBuffer();
	static void DefineDepthStencilStateDisableDepthBuffer();
	
	//blend state
	static void DefineBlendStateDefault();
	static void DefineBlendStateAdditive();
	static void DefineBlendStateNonPremultiplied();
	static void DefineBlendStateAlphaBlend();
	static void DefineBlendStateOpaque();

	//sampler state
	static void DefineSamplerStatePointWrap(UINT slot);
	static void DefineSamplerStateBilinearWrap(UINT slot);
	static void DefineSamplerStateTrilinearWrap(UINT slot);
	static void DefineSamplerStateAnisotropicWrap(UINT slot);

	static void DefineSamplerStatePointClamp(UINT slot);
	static void DefineSamplerStateBilinearClamp(UINT slot);
	static void DefineSamplerStateTrilinearClamp(UINT slot);
	static void DefineSamplerStateAnisotropicClamp(UINT slot);

	static void DefineSamplerStatePointBorder(UINT slot);
	static void DefineSamplerStateBilinearBorder(UINT slot);
	static void DefineSamplerStateTrilinearBorder(UINT slot);
	static void DefineSamplerStateAnisotropicBorder(UINT slot);

	static void DefineViewPort(UINT width, UINT height, FLOAT minDepth, FLOAT maxDepth, UINT topLeftX, UINT topLeftY, D3D11_VIEWPORT *vp);

	static void Release();
};

#endif