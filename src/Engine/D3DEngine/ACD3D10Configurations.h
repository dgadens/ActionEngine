
#ifndef __ACD3D10CONFIGURATIONS_H
#define __ACD3D10CONFIGURATIONS_H

#include <d3d10.h>
#include <d3dx10.h>
#include <DXGI.h>
#include <WTypes.h>

#include "EngineCommon.h"
#include "ACD3D10Globals.h"

class ACD3D10Configurations
{
	//rasterization
	static ID3D10RasterizerState* pCurrentRasterState;
	static ID3D10RasterizerState* pRasterStateSolidCullCCW;
	static ID3D10RasterizerState* pRasterStateWireFrameCullCCW;
	static ID3D10RasterizerState* pRasterStateSolidCullCW;
	static ID3D10RasterizerState* pRasterStateWireFrameCullCW;
	static ID3D10RasterizerState* pRasterStateSolidCullNone;
	static ID3D10RasterizerState* pRasterStateWireFrameCullNone;

	//depth state
	static ID3D10DepthStencilState* pCurrentDepthStencilState;
	static ID3D10DepthStencilState* pDepthStencilStateEnableDepthBuffer;
	static ID3D10DepthStencilState* pDepthStencilStateDisableDepthBuffer;

	//blend state
	static ID3D10BlendState* pCurrentBlendState;
	static ID3D10BlendState* pBlendStateDefault;
	static ID3D10BlendState* pBlendStateAdditive;
	static ID3D10BlendState* pBlendStateNonPremultiplied;
	static ID3D10BlendState* pBlendStateAlphaBlend;
	static ID3D10BlendState* pBlendStateOpaque;

	//sampler state
	static ID3D10SamplerState* pCurrentSamplerState[16];
	static ID3D10SamplerState* pSamplerStatePointWrap;
	static ID3D10SamplerState* pSamplerStateBilinearWrap;
	static ID3D10SamplerState* pSamplerStateTrilinearWrap;
	static ID3D10SamplerState* pSamplerStateAnisotropicWrap;
	static ID3D10SamplerState* pSamplerStatePointClamp;
	static ID3D10SamplerState* pSamplerStateBilinearClamp;
	static ID3D10SamplerState* pSamplerStateTrilinearClamp;
	static ID3D10SamplerState* pSamplerStateAnisotropicClamp;
	static ID3D10SamplerState* pSamplerStatePointBorder;
	static ID3D10SamplerState* pSamplerStateBilinearBorder;
	static ID3D10SamplerState* pSamplerStateTrilinearBorder;
	static ID3D10SamplerState* pSamplerStateAnisotropicBorder;

public:
	static DXGI_SWAP_CHAIN_DESC DefineSwapShain(const HWND hWnd, UINT width, UINT height, UINT numerator, UINT denominator);
	static HRESULT DefineDepthStencilView(ID3D10Device *pDevice, ID3D10Texture2D** pDepthStencil, ID3D10DepthStencilView** pDepthStencilView, UINT width, UINT height);
	
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

	static void DefineViewPort(UINT width, UINT height, FLOAT minDepth, FLOAT maxDepth, UINT topLeftX, UINT topLeftY, D3D10_VIEWPORT *vp);

	static void Release();
};

#endif