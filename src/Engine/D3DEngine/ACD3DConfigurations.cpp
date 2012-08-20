#include "ACD3DConfigurations.h"
#include "ACD3DVertexManager.h"

//rasterization
ID3D11RasterizerState* ACD3DConfigurations::pCurrentRasterState = nullptr;
ID3D11RasterizerState* ACD3DConfigurations::pRasterStateSolidCullCCW = nullptr;
ID3D11RasterizerState* ACD3DConfigurations::pRasterStateWireFrameCullCCW = nullptr;
ID3D11RasterizerState* ACD3DConfigurations::pRasterStateSolidCullCW = nullptr;
ID3D11RasterizerState* ACD3DConfigurations::pRasterStateWireFrameCullCW = nullptr;
ID3D11RasterizerState* ACD3DConfigurations::pRasterStateSolidCullNone = nullptr;
ID3D11RasterizerState* ACD3DConfigurations::pRasterStateWireFrameCullNone = nullptr;

//depth state
ID3D11DepthStencilState* ACD3DConfigurations::pCurrentDepthStencilState = nullptr;
ID3D11DepthStencilState* ACD3DConfigurations::pDepthStencilStateEnableDepthBuffer = nullptr;
ID3D11DepthStencilState* ACD3DConfigurations::pDepthStencilStateDisableDepthBuffer = nullptr;

//blend state
ID3D11BlendState* ACD3DConfigurations::pCurrentBlendState = nullptr;
ID3D11BlendState* ACD3DConfigurations::pBlendStateDefault = nullptr;
ID3D11BlendState* ACD3DConfigurations::pBlendStateAdditive = nullptr;
ID3D11BlendState* ACD3DConfigurations::pBlendStateNonPremultiplied = nullptr;
ID3D11BlendState* ACD3DConfigurations::pBlendStateAlphaBlend = nullptr;
ID3D11BlendState* ACD3DConfigurations::pBlendStateOpaque = nullptr;

//sampler state
ID3D11SamplerState* ACD3DConfigurations::pCurrentSamplerState[16];
ID3D11SamplerState* ACD3DConfigurations::pSamplerStatePointWrap = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateBilinearWrap = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateTrilinearWrap = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateAnisotropicWrap = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStatePointClamp = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateBilinearClamp = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateTrilinearClamp = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateAnisotropicClamp = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStatePointBorder = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateBilinearBorder = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateTrilinearBorder = nullptr;
ID3D11SamplerState* ACD3DConfigurations::pSamplerStateAnisotropicBorder = nullptr;

//define o swapshain
DXGI_SWAP_CHAIN_DESC ACD3DConfigurations::DefineSwapShain(const HWND hWnd, UINT width, UINT height, UINT numerator, UINT denominator)
{
	//cria a descricao do swapchain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = numerator;
	sd.BufferDesc.RefreshRate.Denominator = denominator;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //pode ir para fullscreen

	return sd;
};

HRESULT ACD3DConfigurations::DefineDepthStencilView(ID3D11Device* gDevice, ID3D11Texture2D** pDepthStencil, ID3D11DepthStencilView** pDepthStencilView, UINT width, UINT height)
{
	HRESULT hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = gDevice->CreateTexture2D( &descDepth, nullptr, pDepthStencil );
	if( FAILED( hr ) )
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory ( &descDSV, sizeof ( D3D11_DEPTH_STENCIL_VIEW_DESC ));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = gDevice->CreateDepthStencilView( *pDepthStencil, &descDSV, pDepthStencilView );
	if( FAILED( hr ) )
		return hr;

	return AC_OK;
};

#pragma region RASTERIZARION

//define o rasterizador default
void ACD3DConfigurations::DefineRasterizeStateSolidCullCCW()
{
	if (pRasterStateSolidCullCCW == nullptr)
	{
		D3D11_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D11_FILL_SOLID;
		rasterizerState.CullMode = D3D11_CULL_BACK;
		rasterizerState.FrontCounterClockwise =	true;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3DGlobals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateSolidCullCCW );
	}

	if (pCurrentRasterState != pRasterStateSolidCullCCW)
	{
		pCurrentRasterState = pRasterStateSolidCullCCW;
		ACD3DGlobals::G_pContext->RSSetState(pRasterStateSolidCullCCW);
	}
};

void ACD3DConfigurations::DefineRasterizeStateWireframeCullCCW()
{
	if (pRasterStateWireFrameCullCCW == nullptr)
	{
		D3D11_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerState.CullMode = D3D11_CULL_BACK;
		rasterizerState.FrontCounterClockwise =	true;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3DGlobals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateWireFrameCullCCW );
	}

	if (pCurrentRasterState != pRasterStateWireFrameCullCCW)
	{
		pCurrentRasterState = pRasterStateWireFrameCullCCW;
		ACD3DGlobals::G_pContext->RSSetState(pRasterStateWireFrameCullCCW);
	}
};

void ACD3DConfigurations::DefineRasterizeStateSolidCullCW()
{
	if (pRasterStateSolidCullCW == nullptr)
	{
		D3D11_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D11_FILL_SOLID;
		rasterizerState.CullMode = D3D11_CULL_FRONT;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3DGlobals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateSolidCullCW );
	}

	if (pCurrentRasterState != pRasterStateSolidCullCW)
	{
		pCurrentRasterState = pRasterStateSolidCullCW;
		ACD3DGlobals::G_pContext->RSSetState(pRasterStateSolidCullCW);
	}
};

void ACD3DConfigurations::DefineRasterizeStateWireframeCullCW()
{
	if (pRasterStateWireFrameCullCW == nullptr)
	{
		D3D11_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerState.CullMode = D3D11_CULL_FRONT;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3DGlobals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateWireFrameCullCW );
	}

	if (pCurrentRasterState != pRasterStateWireFrameCullCW)
	{
		pCurrentRasterState = pRasterStateWireFrameCullCW;
		ACD3DGlobals::G_pContext->RSSetState(pRasterStateWireFrameCullCW);
	}
};

void ACD3DConfigurations::DefineRasterizeStateSolidCullNone()
{
	if (pRasterStateSolidCullNone == nullptr)
	{
		D3D11_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D11_FILL_SOLID;
		rasterizerState.CullMode = D3D11_CULL_NONE;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3DGlobals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateSolidCullNone );
	}

	if (pCurrentRasterState != pRasterStateSolidCullNone)
	{
		pCurrentRasterState = pRasterStateSolidCullNone;
		ACD3DGlobals::G_pContext->RSSetState(pRasterStateSolidCullNone);
	}
};

void ACD3DConfigurations::DefineRasterizeStateWireframeCullNone()
{
	if (pRasterStateWireFrameCullNone == nullptr)
	{
		D3D11_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerState.CullMode = D3D11_CULL_NONE;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3DGlobals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateWireFrameCullNone );
	}

	if (pCurrentRasterState != pRasterStateWireFrameCullNone)
	{
		pCurrentRasterState = pRasterStateWireFrameCullNone;
		ACD3DGlobals::G_pContext->RSSetState(pRasterStateWireFrameCullNone);
	}
};

#pragma endregion

#pragma region DEPTHSTATE
//define o rasterizador default
void ACD3DConfigurations::DefineDepthStencilStateEnableDepthBuffer()
{
	if (pDepthStencilStateEnableDepthBuffer == nullptr)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		D3D11_DEPTH_STENCILOP_DESC depthOP;
		depthOP.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthOP.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthOP.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthOP.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.FrontFace = depthOP;
		depthStencilDesc.BackFace = depthOP;

		ACD3DGlobals::G_pD3dDevice->CreateDepthStencilState( &depthStencilDesc, &pDepthStencilStateEnableDepthBuffer );
	}

	if (pCurrentDepthStencilState != pDepthStencilStateEnableDepthBuffer)
	{
		pCurrentDepthStencilState = pDepthStencilStateEnableDepthBuffer;
		ACD3DGlobals::G_pContext->OMSetDepthStencilState(pDepthStencilStateEnableDepthBuffer, 0);
	}
};

//define o rasterizador default
void ACD3DConfigurations::DefineDepthStencilStateDisableDepthBuffer()
{
	if (pDepthStencilStateDisableDepthBuffer == nullptr)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		D3D11_DEPTH_STENCILOP_DESC depthOP;
		depthOP.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthOP.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthOP.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthOP.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.FrontFace = depthOP;
		depthStencilDesc.BackFace = depthOP;

		ACD3DGlobals::G_pD3dDevice->CreateDepthStencilState( &depthStencilDesc, &pDepthStencilStateDisableDepthBuffer );
	}

	if (pCurrentDepthStencilState != pDepthStencilStateDisableDepthBuffer)
	{
		pCurrentDepthStencilState = pDepthStencilStateDisableDepthBuffer;
		ACD3DGlobals::G_pContext->OMSetDepthStencilState(pDepthStencilStateDisableDepthBuffer, 0);
	}
};
#pragma endregion

#pragma region BLENDSTATE
void ACD3DConfigurations::DefineBlendStateDefault()
{
	if (pBlendStateDefault == nullptr)
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;
		rtbDesc.BlendEnable = FALSE;
		rtbDesc.SrcBlend = D3D11_BLEND_ONE;
		rtbDesc.DestBlend = D3D11_BLEND_ZERO;
		rtbDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtbDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbDesc;

		ACD3DGlobals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateDefault);
	}

	if (pCurrentBlendState != pBlendStateDefault)
	{
		pCurrentBlendState = pBlendStateDefault;
		ACD3DGlobals::G_pContext->OMSetBlendState(pBlendStateDefault, 0, 0xffffffff);
	}
};

void ACD3DConfigurations::DefineBlendStateAdditive()
{
	if (pBlendStateAdditive == nullptr)
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;

		rtbDesc.BlendEnable = TRUE;
		rtbDesc.SrcBlend = D3D11_BLEND_ONE;
		rtbDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbDesc.DestBlend = D3D11_BLEND_ONE;
		rtbDesc.DestBlendAlpha = D3D11_BLEND_ONE;
		rtbDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP_MAX;
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbDesc;

		ACD3DGlobals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateAdditive);
	}

	if (pCurrentBlendState != pBlendStateAdditive)
	{
		pCurrentBlendState = pBlendStateAdditive;
		ACD3DGlobals::G_pContext->OMSetBlendState(pBlendStateAdditive, 0, 0xffffffff);
	}
};

void ACD3DConfigurations::DefineBlendStateNonPremultiplied()
{
	if (pBlendStateNonPremultiplied == nullptr)
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;

		rtbDesc.BlendEnable = TRUE;
		rtbDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtbDesc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		rtbDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtbDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		rtbDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbDesc;

		ACD3DGlobals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateNonPremultiplied);
	}

	if (pCurrentBlendState != pBlendStateNonPremultiplied)
	{
		pCurrentBlendState = pBlendStateNonPremultiplied;
		ACD3DGlobals::G_pContext->OMSetBlendState(pBlendStateNonPremultiplied, 0, 0xffffffff);
	}
};

void ACD3DConfigurations::DefineBlendStateAlphaBlend()
{
	if (pBlendStateAlphaBlend == nullptr)
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;

		rtbDesc.BlendEnable = TRUE;
		rtbDesc.SrcBlend = D3D11_BLEND_ONE;
		rtbDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtbDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		rtbDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbDesc;

		ACD3DGlobals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateAlphaBlend);
	}

	if (pCurrentBlendState != pBlendStateAlphaBlend)
	{
		pCurrentBlendState = pBlendStateAlphaBlend;
		ACD3DGlobals::G_pContext->OMSetBlendState(pBlendStateAlphaBlend, 0, 0xffffffff);
	}
};

void ACD3DConfigurations::DefineBlendStateOpaque()
{
	if (pBlendStateOpaque == nullptr)
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;

		rtbDesc.BlendEnable = FALSE;
		rtbDesc.SrcBlend = D3D11_BLEND_ONE;
		rtbDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbDesc.DestBlend = D3D11_BLEND_ZERO;
		rtbDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbDesc;

		ACD3DGlobals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateOpaque);
	}

	if (pCurrentBlendState != pBlendStateOpaque)
	{
		pCurrentBlendState = pBlendStateOpaque;
		ACD3DGlobals::G_pContext->OMSetBlendState(pBlendStateOpaque, 0, 0xffffffff);
	}
};

#pragma endregion

#pragma region SAMPLERSTATE
void ACD3DConfigurations::DefineSamplerStatePointWrap(UINT slot)
{
	if (pSamplerStatePointWrap == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStatePointWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStatePointWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStatePointWrap;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStatePointWrap);
	}
};

void ACD3DConfigurations::DefineSamplerStateBilinearWrap(UINT slot)
{
	if (pSamplerStateBilinearWrap == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateBilinearWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateBilinearWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStateBilinearWrap;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateBilinearWrap);
	}
};

void ACD3DConfigurations::DefineSamplerStateTrilinearWrap(UINT slot)
{
	if (pSamplerStateTrilinearWrap == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateTrilinearWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateTrilinearWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStateTrilinearWrap;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateTrilinearWrap);
	}
};

void ACD3DConfigurations::DefineSamplerStateAnisotropicWrap(UINT slot)
{
	if (pSamplerStateTrilinearWrap == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateAnisotropicWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateAnisotropicWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStateAnisotropicWrap;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateAnisotropicWrap);
	}
};


void ACD3DConfigurations::DefineSamplerStatePointClamp(UINT slot)
{
	if (pSamplerStatePointClamp == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStatePointClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStatePointClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStatePointClamp;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStatePointClamp);
	}
};

void ACD3DConfigurations::DefineSamplerStateBilinearClamp(UINT slot)
{
	if (pSamplerStateBilinearClamp == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateBilinearClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateBilinearClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStateBilinearClamp;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateBilinearClamp);
	}
};

void ACD3DConfigurations::DefineSamplerStateTrilinearClamp(UINT slot)
{
	if (pSamplerStateTrilinearClamp == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateTrilinearClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateTrilinearClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStateTrilinearClamp;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateTrilinearClamp);
	}
};

void ACD3DConfigurations::DefineSamplerStateAnisotropicClamp(UINT slot)
{
	if (pSamplerStateTrilinearClamp == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateAnisotropicClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateAnisotropicClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStateAnisotropicClamp;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateAnisotropicClamp);
	}
};

void ACD3DConfigurations::DefineSamplerStatePointBorder(UINT slot)
{
	if (pSamplerStatePointBorder == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStatePointBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStatePointBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStatePointBorder;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStatePointBorder);
	}
};

void ACD3DConfigurations::DefineSamplerStateBilinearBorder(UINT slot)
{
	if (pSamplerStateBilinearBorder == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateBilinearBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateBilinearBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStateBilinearBorder;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateBilinearBorder);
	}
};

void ACD3DConfigurations::DefineSamplerStateTrilinearBorder(UINT slot)
{
	if (pSamplerStateTrilinearBorder == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateTrilinearBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateTrilinearBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStateTrilinearBorder;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateTrilinearBorder);
	}
};

void ACD3DConfigurations::DefineSamplerStateAnisotropicBorder(UINT slot)
{
	if (pSamplerStateTrilinearBorder == nullptr)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3DGlobals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateAnisotropicBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateAnisotropicBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStateAnisotropicBorder;
		ACD3DGlobals::G_pContext->PSSetSamplers(slot, 1, &pSamplerStateAnisotropicBorder);
	}
};

#pragma endregion

//define a viewport
void ACD3DConfigurations::DefineViewPort(UINT width, UINT height, FLOAT minDepth, FLOAT maxDepth, UINT topLeftX, UINT topLeftY, D3D11_VIEWPORT *vp)
{
	vp->Width = width;
	vp->Height = height;
	vp->MinDepth = minDepth;
	vp->MaxDepth = maxDepth;
	vp->TopLeftX = topLeftX;
	vp->TopLeftY = topLeftY;
};

void ACD3DConfigurations::Release()
{
	//rasterization
	SAFE_RELEASE(pRasterStateSolidCullCCW);
	SAFE_RELEASE(pRasterStateWireFrameCullCCW);
	SAFE_RELEASE(pRasterStateSolidCullCW);
	SAFE_RELEASE(pRasterStateWireFrameCullCW);
	SAFE_RELEASE(pRasterStateSolidCullNone);
	SAFE_RELEASE(pRasterStateWireFrameCullNone);

	//depth state
	SAFE_RELEASE(pDepthStencilStateEnableDepthBuffer);
	SAFE_RELEASE(pDepthStencilStateDisableDepthBuffer);

	//blend state
	SAFE_RELEASE(pBlendStateDefault);
	SAFE_RELEASE(pBlendStateAdditive);
	SAFE_RELEASE(pBlendStateNonPremultiplied);
	SAFE_RELEASE(pBlendStateAlphaBlend);
	SAFE_RELEASE(pBlendStateOpaque);

	//sampler state
	SAFE_RELEASE(pSamplerStatePointWrap);
	SAFE_RELEASE(pSamplerStateBilinearWrap);
	SAFE_RELEASE(pSamplerStateTrilinearWrap);
	SAFE_RELEASE(pSamplerStateAnisotropicWrap);
	SAFE_RELEASE(pSamplerStatePointClamp);
	SAFE_RELEASE(pSamplerStateBilinearClamp);
	SAFE_RELEASE(pSamplerStateTrilinearClamp);
	SAFE_RELEASE(pSamplerStateAnisotropicClamp);
	SAFE_RELEASE(pSamplerStatePointBorder);
	SAFE_RELEASE(pSamplerStateBilinearBorder);
	SAFE_RELEASE(pSamplerStateTrilinearBorder);
	SAFE_RELEASE(pSamplerStateAnisotropicBorder);
};

