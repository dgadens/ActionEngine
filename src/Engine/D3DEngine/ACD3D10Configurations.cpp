#include "ACD3D10Configurations.h"
#include "ACD3D10VertexManager.h"

//rasterization
ID3D10RasterizerState* ACD3D10Configurations::pCurrentRasterState = nullptr;
ID3D10RasterizerState* ACD3D10Configurations::pRasterStateSolidCullCCW = nullptr;
ID3D10RasterizerState* ACD3D10Configurations::pRasterStateWireFrameCullCCW = nullptr;
ID3D10RasterizerState* ACD3D10Configurations::pRasterStateSolidCullCW = nullptr;
ID3D10RasterizerState* ACD3D10Configurations::pRasterStateWireFrameCullCW = nullptr;
ID3D10RasterizerState* ACD3D10Configurations::pRasterStateSolidCullNone = nullptr;
ID3D10RasterizerState* ACD3D10Configurations::pRasterStateWireFrameCullNone = nullptr;

//depth state
ID3D10DepthStencilState* ACD3D10Configurations::pCurrentDepthStencilState = nullptr;
ID3D10DepthStencilState* ACD3D10Configurations::pDepthStencilStateEnableDepthBuffer = nullptr;
ID3D10DepthStencilState* ACD3D10Configurations::pDepthStencilStateDisableDepthBuffer = nullptr;

//blend state
ID3D10BlendState* ACD3D10Configurations::pCurrentBlendState = nullptr;
ID3D10BlendState* ACD3D10Configurations::pBlendStateDefault = nullptr;
ID3D10BlendState* ACD3D10Configurations::pBlendStateAdditive = nullptr;
ID3D10BlendState* ACD3D10Configurations::pBlendStateNonPremultiplied = nullptr;
ID3D10BlendState* ACD3D10Configurations::pBlendStateAlphaBlend = nullptr;
ID3D10BlendState* ACD3D10Configurations::pBlendStateOpaque = nullptr;

//sampler state
ID3D10SamplerState* ACD3D10Configurations::pCurrentSamplerState[16];
ID3D10SamplerState* ACD3D10Configurations::pSamplerStatePointWrap = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateBilinearWrap = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateTrilinearWrap = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateAnisotropicWrap = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStatePointClamp = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateBilinearClamp = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateTrilinearClamp = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateAnisotropicClamp = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStatePointBorder = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateBilinearBorder = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateTrilinearBorder = nullptr;
ID3D10SamplerState* ACD3D10Configurations::pSamplerStateAnisotropicBorder = nullptr;

//define o swapshain
DXGI_SWAP_CHAIN_DESC ACD3D10Configurations::DefineSwapShain(const HWND hWnd, UINT width, UINT height, UINT numerator, UINT denominator)
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

HRESULT ACD3D10Configurations::DefineDepthStencilView(ID3D10Device* gDevice, ID3D10Texture2D** pDepthStencil, ID3D10DepthStencilView** pDepthStencilView, UINT width, UINT height)
{
	HRESULT hr;

	// Create depth stencil texture
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = gDevice->CreateTexture2D( &descDepth, nullptr, pDepthStencil );
	if( FAILED( hr ) )
		return hr;

	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = gDevice->CreateDepthStencilView( *pDepthStencil, &descDSV, pDepthStencilView );
	if( FAILED( hr ) )
		return hr;

	return AC_OK;
};

#pragma region RASTERIZARION

//define o rasterizador default
void ACD3D10Configurations::DefineRasterizeStateSolidCullCCW()
{
	if (pRasterStateSolidCullCCW == nullptr)
	{
		D3D10_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D10_FILL_SOLID;
		rasterizerState.CullMode = D3D10_CULL_BACK;
		rasterizerState.FrontCounterClockwise =	true;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3D10Globals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateSolidCullCCW );
	}

	if (pCurrentRasterState != pRasterStateSolidCullCCW)
	{
		pCurrentRasterState = pRasterStateSolidCullCCW;
		ACD3D10Globals::G_pD3dDevice->RSSetState(pRasterStateSolidCullCCW);
	}
};

void ACD3D10Configurations::DefineRasterizeStateWireframeCullCCW()
{
	if (pRasterStateWireFrameCullCCW == nullptr)
	{
		D3D10_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D10_FILL_WIREFRAME;
		rasterizerState.CullMode = D3D10_CULL_BACK;
		rasterizerState.FrontCounterClockwise =	true;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3D10Globals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateWireFrameCullCCW );
	}

	if (pCurrentRasterState != pRasterStateWireFrameCullCCW)
	{
		pCurrentRasterState = pRasterStateWireFrameCullCCW;
		ACD3D10Globals::G_pD3dDevice->RSSetState(pRasterStateWireFrameCullCCW);
	}
};

void ACD3D10Configurations::DefineRasterizeStateSolidCullCW()
{
	if (pRasterStateSolidCullCW == nullptr)
	{
		D3D10_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D10_FILL_SOLID;
		rasterizerState.CullMode = D3D10_CULL_FRONT;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3D10Globals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateSolidCullCW );
	}

	if (pCurrentRasterState != pRasterStateSolidCullCW)
	{
		pCurrentRasterState = pRasterStateSolidCullCW;
		ACD3D10Globals::G_pD3dDevice->RSSetState(pRasterStateSolidCullCW);
	}
};

void ACD3D10Configurations::DefineRasterizeStateWireframeCullCW()
{
	if (pRasterStateWireFrameCullCW == nullptr)
	{
		D3D10_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D10_FILL_WIREFRAME;
		rasterizerState.CullMode = D3D10_CULL_FRONT;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3D10Globals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateWireFrameCullCW );
	}

	if (pCurrentRasterState != pRasterStateWireFrameCullCW)
	{
		pCurrentRasterState = pRasterStateWireFrameCullCW;
		ACD3D10Globals::G_pD3dDevice->RSSetState(pRasterStateWireFrameCullCW);
	}
};

void ACD3D10Configurations::DefineRasterizeStateSolidCullNone()
{
	if (pRasterStateSolidCullNone == nullptr)
	{
		D3D10_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D10_FILL_SOLID;
		rasterizerState.CullMode = D3D10_CULL_NONE;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3D10Globals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateSolidCullNone );
	}

	if (pCurrentRasterState != pRasterStateSolidCullNone)
	{
		pCurrentRasterState = pRasterStateSolidCullNone;
		ACD3D10Globals::G_pD3dDevice->RSSetState(pRasterStateSolidCullNone);
	}
};

void ACD3D10Configurations::DefineRasterizeStateWireframeCullNone()
{
	if (pRasterStateWireFrameCullNone == nullptr)
	{
		D3D10_RASTERIZER_DESC rasterizerState;
		rasterizerState.FillMode = D3D10_FILL_WIREFRAME;
		rasterizerState.CullMode = D3D10_CULL_NONE;
		rasterizerState.FrontCounterClockwise =	false;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.MultisampleEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		ACD3D10Globals::G_pD3dDevice->CreateRasterizerState( &rasterizerState, &pRasterStateWireFrameCullNone );
	}

	if (pCurrentRasterState != pRasterStateWireFrameCullNone)
	{
		pCurrentRasterState = pRasterStateWireFrameCullNone;
		ACD3D10Globals::G_pD3dDevice->RSSetState(pRasterStateWireFrameCullNone);
	}
};

#pragma endregion

#pragma region DEPTHSTATE
//define o rasterizador default
void ACD3D10Configurations::DefineDepthStencilStateEnableDepthBuffer()
{
	if (pDepthStencilStateEnableDepthBuffer == nullptr)
	{
		D3D10_DEPTH_STENCIL_DESC depthStencilDesc;

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

		D3D10_DEPTH_STENCILOP_DESC depthOP;
		depthOP.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthOP.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
		depthOP.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthOP.StencilFunc = D3D10_COMPARISON_ALWAYS;

		depthStencilDesc.FrontFace = depthOP;
		depthStencilDesc.BackFace = depthOP;

		ACD3D10Globals::G_pD3dDevice->CreateDepthStencilState( &depthStencilDesc, &pDepthStencilStateEnableDepthBuffer );
	}

	if (pCurrentDepthStencilState != pDepthStencilStateEnableDepthBuffer)
	{
		pCurrentDepthStencilState = pDepthStencilStateEnableDepthBuffer;
		ACD3D10Globals::G_pD3dDevice->OMSetDepthStencilState(pDepthStencilStateEnableDepthBuffer, 0);
	}
};

//define o rasterizador default
void ACD3D10Configurations::DefineDepthStencilStateDisableDepthBuffer()
{
	if (pDepthStencilStateDisableDepthBuffer == nullptr)
	{
		D3D10_DEPTH_STENCIL_DESC depthStencilDesc;

		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

		D3D10_DEPTH_STENCILOP_DESC depthOP;
		depthOP.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthOP.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
		depthOP.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthOP.StencilFunc = D3D10_COMPARISON_ALWAYS;

		depthStencilDesc.FrontFace = depthOP;
		depthStencilDesc.BackFace = depthOP;

		ACD3D10Globals::G_pD3dDevice->CreateDepthStencilState( &depthStencilDesc, &pDepthStencilStateDisableDepthBuffer );
	}

	if (pCurrentDepthStencilState != pDepthStencilStateDisableDepthBuffer)
	{
		pCurrentDepthStencilState = pDepthStencilStateDisableDepthBuffer;
		ACD3D10Globals::G_pD3dDevice->OMSetDepthStencilState(pDepthStencilStateDisableDepthBuffer, 0);
	}
};
#pragma endregion

#pragma region BLENDSTATE
void ACD3D10Configurations::DefineBlendStateDefault()
{
	if (pBlendStateDefault == nullptr)
	{
		D3D10_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.BlendEnable[0] = FALSE;
		blendDesc.SrcBlend = D3D10_BLEND_ONE;
		blendDesc.DestBlend = D3D10_BLEND_ZERO;
		blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
		blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

		ACD3D10Globals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateDefault);
	}

	if (pCurrentBlendState != pBlendStateDefault)
	{
		pCurrentBlendState = pBlendStateDefault;
		ACD3D10Globals::G_pD3dDevice->OMSetBlendState(pBlendStateDefault, 0, 0xffffffff);
	}
};

void ACD3D10Configurations::DefineBlendStateAdditive()
{
	if (pBlendStateAdditive == nullptr)
	{
		D3D10_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.BlendEnable[0] = TRUE;
		blendDesc.SrcBlend = D3D10_BLEND_ONE;
		blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
		blendDesc.DestBlend = D3D10_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D10_BLEND_ONE;
		blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
		blendDesc.BlendOpAlpha = D3D10_BLEND_OP_MAX;

		blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

		ACD3D10Globals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateAdditive);
	}

	if (pCurrentBlendState != pBlendStateAdditive)
	{
		pCurrentBlendState = pBlendStateAdditive;
		ACD3D10Globals::G_pD3dDevice->OMSetBlendState(pBlendStateAdditive, 0, 0xffffffff);
	}
};

void ACD3D10Configurations::DefineBlendStateNonPremultiplied()
{
	if (pBlendStateNonPremultiplied == nullptr)
	{
		D3D10_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.BlendEnable[0] = TRUE;
		blendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
		blendDesc.SrcBlendAlpha = D3D10_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
		blendDesc.DestBlendAlpha = D3D10_BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
		blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

		ACD3D10Globals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateNonPremultiplied);
	}

	if (pCurrentBlendState != pBlendStateNonPremultiplied)
	{
		pCurrentBlendState = pBlendStateNonPremultiplied;
		ACD3D10Globals::G_pD3dDevice->OMSetBlendState(pBlendStateNonPremultiplied, 0, 0xffffffff);
	}
};

void ACD3D10Configurations::DefineBlendStateAlphaBlend()
{
	if (pBlendStateAlphaBlend == nullptr)
	{
		D3D10_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.BlendEnable[0] = TRUE;
		blendDesc.SrcBlend = D3D10_BLEND_ONE;
		blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
		blendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
		blendDesc.DestBlendAlpha = D3D10_BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
		blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

		ACD3D10Globals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateAlphaBlend);
	}

	if (pCurrentBlendState != pBlendStateAlphaBlend)
	{
		pCurrentBlendState = pBlendStateAlphaBlend;
		ACD3D10Globals::G_pD3dDevice->OMSetBlendState(pBlendStateAlphaBlend, 0, 0xffffffff);
	}
};

void ACD3D10Configurations::DefineBlendStateOpaque()
{
	if (pBlendStateOpaque == nullptr)
	{
		D3D10_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.BlendEnable[0] = FALSE;
		blendDesc.SrcBlend = D3D10_BLEND_ONE;
		blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
		blendDesc.DestBlend = D3D10_BLEND_ZERO;
		blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
		blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
		blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

		ACD3D10Globals::G_pD3dDevice->CreateBlendState(&blendDesc, &pBlendStateOpaque);
	}

	if (pCurrentBlendState != pBlendStateOpaque)
	{
		pCurrentBlendState = pBlendStateOpaque;
		ACD3D10Globals::G_pD3dDevice->OMSetBlendState(pBlendStateOpaque, 0, 0xffffffff);
	}
};

#pragma endregion

#pragma region SAMPLERSTATE
void ACD3D10Configurations::DefineSamplerStatePointWrap(UINT slot)
{
	if (pSamplerStatePointWrap == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStatePointWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStatePointWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStatePointWrap;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStatePointWrap);
	}
};

void ACD3D10Configurations::DefineSamplerStateBilinearWrap(UINT slot)
{
	if (pSamplerStateBilinearWrap == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateBilinearWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateBilinearWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStateBilinearWrap;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateBilinearWrap);
	}
};

void ACD3D10Configurations::DefineSamplerStateTrilinearWrap(UINT slot)
{
	if (pSamplerStateTrilinearWrap == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateTrilinearWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateTrilinearWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStateTrilinearWrap;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateTrilinearWrap);
	}
};

void ACD3D10Configurations::DefineSamplerStateAnisotropicWrap(UINT slot)
{
	if (pSamplerStateTrilinearWrap == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateAnisotropicWrap);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateAnisotropicWrap)
	{
		pCurrentSamplerState[slot] = pSamplerStateAnisotropicWrap;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateAnisotropicWrap);
	}
};


void ACD3D10Configurations::DefineSamplerStatePointClamp(UINT slot)
{
	if (pSamplerStatePointClamp == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStatePointClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStatePointClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStatePointClamp;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStatePointClamp);
	}
};

void ACD3D10Configurations::DefineSamplerStateBilinearClamp(UINT slot)
{
	if (pSamplerStateBilinearClamp == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateBilinearClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateBilinearClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStateBilinearClamp;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateBilinearClamp);
	}
};

void ACD3D10Configurations::DefineSamplerStateTrilinearClamp(UINT slot)
{
	if (pSamplerStateTrilinearClamp == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateTrilinearClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateTrilinearClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStateTrilinearClamp;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateTrilinearClamp);
	}
};

void ACD3D10Configurations::DefineSamplerStateAnisotropicClamp(UINT slot)
{
	if (pSamplerStateTrilinearClamp == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateAnisotropicClamp);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateAnisotropicClamp)
	{
		pCurrentSamplerState[slot] = pSamplerStateAnisotropicClamp;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateAnisotropicClamp);
	}
};

void ACD3D10Configurations::DefineSamplerStatePointBorder(UINT slot)
{
	if (pSamplerStatePointBorder == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStatePointBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStatePointBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStatePointBorder;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStatePointBorder);
	}
};

void ACD3D10Configurations::DefineSamplerStateBilinearBorder(UINT slot)
{
	if (pSamplerStateBilinearBorder == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateBilinearBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateBilinearBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStateBilinearBorder;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateBilinearBorder);
	}
};

void ACD3D10Configurations::DefineSamplerStateTrilinearBorder(UINT slot)
{
	if (pSamplerStateTrilinearBorder == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateTrilinearBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateTrilinearBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStateTrilinearBorder;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateTrilinearBorder);
	}
};

void ACD3D10Configurations::DefineSamplerStateAnisotropicBorder(UINT slot)
{
	if (pSamplerStateTrilinearBorder == nullptr)
	{
		D3D10_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D10_SAMPLER_DESC));

		samplerDesc.Filter = D3D10_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

		// Create the texture sampler state.
		ACD3D10Globals::G_pD3dDevice->CreateSamplerState(&samplerDesc, &pSamplerStateAnisotropicBorder);
	}

	if (pCurrentSamplerState[slot] != pSamplerStateAnisotropicBorder)
	{
		pCurrentSamplerState[slot] = pSamplerStateAnisotropicBorder;
		ACD3D10Globals::G_pD3dDevice->PSSetSamplers(slot, 1, &pSamplerStateAnisotropicBorder);
	}
};

#pragma endregion

//define a viewport
void ACD3D10Configurations::DefineViewPort(UINT width, UINT height, FLOAT minDepth, FLOAT maxDepth, UINT topLeftX, UINT topLeftY, D3D10_VIEWPORT *vp)
{
	vp->Width = width;
	vp->Height = height;
	vp->MinDepth = minDepth;
	vp->MaxDepth = maxDepth;
	vp->TopLeftX = topLeftX;
	vp->TopLeftY = topLeftY;
};

void ACD3D10Configurations::Release()
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

